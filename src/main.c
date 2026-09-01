#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/poker.h"

static int perguntarNumJogadores(void) {
    int n = 0;
    do {
        printf("Quantos jogadores no total (2 a %d, incluindo voce)? ", MAX_JOGADORES);
        if (scanf("%d", &n) != 1) { while (getchar() != '\n'); n = 0; }
    } while (n < 2 || n > MAX_JOGADORES);
    return n;
}

static void jogarPartida(void) {
    Jogo jogo;
    int numJogadores = perguntarNumJogadores();
    inicializarJogo(&jogo, numJogadores, numJogadores - 1);

    while (!jogoTerminou(&jogo)) {
        printf("\n\n########## NOVA MAO ##########\n");
        iniciarNovaMao(&jogo);

        printf("\n--- Pre-Flop ---\n");
        mostrarMesa(&jogo);
        rodadaApostas(&jogo, 0);

        if (contarJogadoresAtivos(&jogo) > 1) {
            printf("\n--- Flop ---\n");
            distribuirCartasComunitarias(&jogo, 3);
            mostrarMesa(&jogo);
            rodadaApostas(&jogo, 3);
        }

        if (contarJogadoresAtivos(&jogo) > 1) {
            printf("\n--- Turn ---\n");
            distribuirCartasComunitarias(&jogo, 1);
            mostrarMesa(&jogo);
            rodadaApostas(&jogo, 4);
        }

        if (contarJogadoresAtivos(&jogo) > 1) {
            printf("\n--- River ---\n");
            distribuirCartasComunitarias(&jogo, 1);
            mostrarMesa(&jogo);
            rodadaApostas(&jogo, 5);
        }

        if (contarJogadoresAtivos(&jogo) > 1) {
            distribuirPote(&jogo);
        } else {
            /* Apenas um jogador restante: ele leva o pote sem showdown */
            for (int i = 0; i < jogo.numJogadores; i++) {
                if (jogo.jogadores[i].ativo) {
                    printf("\n>>> %s venceu o pote de %d fichas (demais desistiram)! <<<\n",
                           jogo.jogadores[i].nome, jogo.pote);
                    jogo.jogadores[i].stack += jogo.pote;
                    jogo.pote = 0;
                    break;
                }
            }
        }

        printf("\n--- Placar apos a mao ---\n");
        for (int i = 0; i < jogo.numJogadores; i++)
            printf(" %-8s : %d fichas\n", jogo.jogadores[i].nome, jogo.jogadores[i].stack);

        jogo.dealerPos = (jogo.dealerPos + 1) % jogo.numJogadores;

        if (jogoTerminou(&jogo)) break;

        printf("\nContinuar para a proxima mao? [1] Sim [0] Nao: ");
        int cont;
        if (scanf("%d", &cont) != 1) { while (getchar() != '\n'); cont = 0; }
        if (!cont) break;
    }

    printf("\n===== FIM DA PARTIDA =====\n");
    for (int i = 0; i < jogo.numJogadores; i++)
        printf(" %-8s : %d fichas\n", jogo.jogadores[i].nome, jogo.jogadores[i].stack);
}

int main(void) {
    srand((unsigned int)time(NULL));
    int opcao;
    int rodando = 1;

    while (rodando) {
        exibirMenuPrincipal();
        opcao = lerOpcaoMenu();

        switch (opcao) {
            case 1:
                jogarPartida();
                break;
            case 2:
                exibirRegras();
                break;
            case 3:
                printf("\nAte a proxima!\n");
                rodando = 0;
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                while (getchar() != '\n');
        }
    }
    return 0;
}
