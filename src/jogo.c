#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/poker.h"

void inicializarJogo(Jogo *j, int numJogadores, int numBots) {
    j->numJogadores = numJogadores;
    j->dealerPos = 0;
    j->mesaAtiva = 1;

    for (int i = 0; i < numJogadores; i++) {
        if (i == 0) {
            strcpy(j->jogadores[i].nome, "Voce");
            j->jogadores[i].ehBot = 0;
        } else {
            snprintf(j->jogadores[i].nome, sizeof(j->jogadores[i].nome), "Bot %d", i);
            j->jogadores[i].ehBot = 1;
        }
        j->jogadores[i].stack = STACK_INICIAL;
        j->jogadores[i].ativo = 1;
        j->jogadores[i].allIn = 0;
        j->jogadores[i].apostaRodada = 0;
    }
    (void)numBots;
}

int contarJogadoresAtivos(Jogo *j) {
    int c = 0;
    for (int i = 0; i < j->numJogadores; i++) if (j->jogadores[i].ativo) c++;
    return c;
}

static void resetarRodada(Jogo *j) {
    for (int i = 0; i < j->numJogadores; i++) j->jogadores[i].apostaRodada = 0;
}

void iniciarNovaMao(Jogo *j) {
    inicializarBaralho(&j->baralho);
    embaralhar(&j->baralho);
    j->pote = 0;
    j->numComunitarias = 0;

    for (int i = 0; i < j->numJogadores; i++) {
        j->jogadores[i].apostaRodada = 0;
        j->jogadores[i].allIn = 0;
        j->jogadores[i].ativo = (j->jogadores[i].stack > 0) ? 1 : 0;
    }

    /* distribuir 2 cartas para cada jogador ativo */
    for (int c = 0; c < CARTAS_MAO; c++) {
        for (int i = 0; i < j->numJogadores; i++) {
            if (j->jogadores[i].ativo)
                j->jogadores[i].mao[c] = comprarCarta(&j->baralho);
        }
    }

    /* Blinds simples: joga baseado no dealerPos */
    int sb = (j->dealerPos + 1) % j->numJogadores;
    int bb = (j->dealerPos + 2) % j->numJogadores;

    int valorSB = (j->jogadores[sb].stack < SMALL_BLIND) ? j->jogadores[sb].stack : SMALL_BLIND;
    j->jogadores[sb].stack -= valorSB;
    j->jogadores[sb].apostaRodada = valorSB;
    j->pote += valorSB;

    int valorBB = (j->jogadores[bb].stack < BIG_BLIND) ? j->jogadores[bb].stack : BIG_BLIND;
    j->jogadores[bb].stack -= valorBB;
    j->jogadores[bb].apostaRodada = valorBB;
    j->pote += valorBB;

    printf("\n%s postou o small blind (%d)\n", j->jogadores[sb].nome, valorSB);
    printf("%s postou o big blind (%d)\n", j->jogadores[bb].nome, valorBB);
}

void distribuirCartasComunitarias(Jogo *j, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        j->comunitarias[j->numComunitarias++] = comprarCarta(&j->baralho);
    }
}

void mostrarMesa(Jogo *j) {
    printf("\n===== MESA =====\n");
    printf("Pote atual: %d\n", j->pote);
    printf("Cartas comunitarias: ");
    if (j->numComunitarias == 0) printf("(nenhuma ainda)");
    for (int i = 0; i < j->numComunitarias; i++) {
        imprimirCarta(j->comunitarias[i]);
        printf(" ");
    }
    printf("\n");
    for (int i = 0; i < j->numJogadores; i++) {
        Jogador *p = &j->jogadores[i];
        printf(" - %-8s | stack: %5d | aposta: %4d | %s\n",
               p->nome, p->stack, p->apostaRodada,
               p->ativo ? (p->allIn ? "ALL-IN" : "na mao") : "fora (fold)");
    }
    printf("=================\n");
}

/* Decisão simples de bot: baseada em força aproximada da mão + aleatoriedade */
static int decisaoBot(Jogo *j, int idx, int apostaAtual) {
    Jogador *p = &j->jogadores[idx];
    int precisaPagar = apostaAtual - p->apostaRodada;
    int chance = rand() % 100;

    if (precisaPagar <= 0) {
        /* pode check ou apostar */
        if (chance < 70) return 0; /* check */
        return 1; /* aposta/raise pequeno */
    }
    if (precisaPagar >= p->stack) {
        /* teria que dar all-in */
        return (chance < 40) ? 2 : 3; /* 2=call(all-in), 3=fold */
    }
    if (chance < 15) return 3; /* fold */
    if (chance < 85) return 2; /* call */
    return 1; /* raise */
}

/* Executa uma rodada de apostas simples (uma volta na mesa).
   cartasComunitariasReveladas apenas para fins informativos no print. */
void rodadaApostas(Jogo *j, int cartasComunitariasReveladas) {
    (void)cartasComunitariasReveladas;
    resetarRodada(j);
    int apostaAtual = BIG_BLIND;
    /* Simplificação: já ajusta aposta atual com base no maior valor já postado (blinds) */
    for (int i = 0; i < j->numJogadores; i++)
        if (j->jogadores[i].apostaRodada > apostaAtual) apostaAtual = j->jogadores[i].apostaRodada;

    int rodadasSemAumento = 0;
    int i = (j->dealerPos + 1) % j->numJogadores;
    int voltasMax = j->numJogadores * 3; /* limite de segurança */
    int voltas = 0;

    while (rodadasSemAumento < contarJogadoresAtivos(j) && voltas < voltasMax) {
        Jogador *p = &j->jogadores[i];
        voltas++;

        if (!p->ativo || p->allIn) {
            i = (i + 1) % j->numJogadores;
            continue;
        }
        if (contarJogadoresAtivos(j) <= 1) break;

        int precisaPagar = apostaAtual - p->apostaRodada;

        if (p->ehBot) {
            int decisao = decisaoBot(j, i, apostaAtual);
            if (decisao == 3 && precisaPagar > 0) {
                p->ativo = 0;
                printf("%s desistiu (fold).\n", p->nome);
                rodadasSemAumento++;
            } else if (decisao == 1) {
                int raise = BIG_BLIND;
                int total = precisaPagar + raise;
                if (total >= p->stack) { total = p->stack; p->allIn = 1; }
                p->stack -= total;
                p->apostaRodada += total;
                if (p->apostaRodada > apostaAtual) apostaAtual = p->apostaRodada;
                j->pote += total;
                printf("%s aumentou para %d.\n", p->nome, p->apostaRodada);
                rodadasSemAumento = 1;
            } else {
                int pagar = precisaPagar;
                if (pagar >= p->stack) { pagar = p->stack; p->allIn = 1; }
                p->stack -= pagar;
                p->apostaRodada += pagar;
                j->pote += pagar;
                if (pagar > 0)
                    printf("%s pagou %d.\n", p->nome, pagar);
                else
                    printf("%s passou (check).\n", p->nome);
                rodadasSemAumento++;
            }
        } else {
            /* jogador humano */
            printf("\nSua mao: ");
            imprimirCarta(p->mao[0]); printf(" "); imprimirCarta(p->mao[1]); printf("\n");
            printf("Voce precisa pagar: %d (seu stack: %d)\n", precisaPagar, p->stack);
            printf("Opcoes: [1] Pagar/Check  [2] Aumentar (+%d)  [3] Desistir\n", BIG_BLIND);
            printf("Escolha: ");
            int opc = 0;
            if (scanf("%d", &opc) != 1) { opc = 1; while(getchar()!='\n'); }

            if (opc == 3 && precisaPagar > 0) {
                p->ativo = 0;
                printf("Voce desistiu.\n");
                rodadasSemAumento++;
            } else if (opc == 2) {
                int total = precisaPagar + BIG_BLIND;
                if (total >= p->stack) { total = p->stack; p->allIn = 1; }
                p->stack -= total;
                p->apostaRodada += total;
                if (p->apostaRodada > apostaAtual) apostaAtual = p->apostaRodada;
                j->pote += total;
                printf("Voce aumentou para %d.\n", p->apostaRodada);
                rodadasSemAumento = 1;
            } else {
                int pagar = precisaPagar;
                if (pagar >= p->stack) { pagar = p->stack; p->allIn = 1; }
                p->stack -= pagar;
                p->apostaRodada += pagar;
                j->pote += pagar;
                printf("Voce pagou %d.\n", pagar);
                rodadasSemAumento++;
            }
        }
        i = (i + 1) % j->numJogadores;
    }
}

void distribuirPote(Jogo *j) {
    int vencedor = -1;
    AvaliacaoMao melhorAval;
    memset(&melhorAval, 0, sizeof(melhorAval));
    int primeiro = 1;

    printf("\n===== SHOWDOWN =====\n");
    for (int i = 0; i < j->numJogadores; i++) {
        Jogador *p = &j->jogadores[i];
        if (!p->ativo) continue;

        Carta todas[7];
        todas[0] = p->mao[0];
        todas[1] = p->mao[1];
        for (int c = 0; c < j->numComunitarias; c++) todas[2 + c] = j->comunitarias[c];

        AvaliacaoMao aval = avaliarMao(todas, 2 + j->numComunitarias);
        printf("%s: ", p->nome);
        imprimirCarta(p->mao[0]); printf(" "); imprimirCarta(p->mao[1]);
        printf(" -> %s\n", nomeRanking(aval.ranking));

        if (primeiro || compararMaos(aval, melhorAval) > 0) {
            melhorAval = aval;
            vencedor = i;
            primeiro = 0;
        }
    }

    if (vencedor >= 0) {
        printf("\n>>> %s venceu o pote de %d fichas! <<<\n", j->jogadores[vencedor].nome, j->pote);
        j->jogadores[vencedor].stack += j->pote;
    }
    j->pote = 0;
}

int jogoTerminou(Jogo *j) {
    int comFichas = 0;
    for (int i = 0; i < j->numJogadores; i++)
        if (j->jogadores[i].stack > 0) comFichas++;
    return comFichas <= 1;
}
