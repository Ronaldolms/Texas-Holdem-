#include <stdio.h>
#include <string.h>
#include "../include/poker.h"

/* Ordena vetor de inteiros decrescente (bubble simples, poucos elementos) */
static void ordenarDecrescente(int v[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (v[j] < v[j + 1]) {
                int tmp = v[j]; v[j] = v[j + 1]; v[j + 1] = tmp;
            }
}

/* Gera todas as combinações C(n,5) e avalia a melhor.
   cartas: até 7 cartas (2 da mao + 5 comunitarias) */
static AvaliacaoMao avaliar5(Carta c[5]) {
    int valores[5];
    int naipes[5];
    for (int i = 0; i < 5; i++) { valores[i] = c[i].valor; naipes[i] = c[i].naipe; }
    ordenarDecrescente(valores, 5);

    int flush = 1;
    for (int i = 1; i < 5; i++) if (naipes[i] != naipes[0]) flush = 0;

    /* Verifica sequência (trata também A-2-3-4-5) */
    int seq = 1;
    for (int i = 0; i < 4; i++) {
        if (valores[i] - valores[i + 1] != 1) { seq = 0; break; }
    }
    int valoresSeq[5];
    memcpy(valoresSeq, valores, sizeof(valores));
    if (!seq) {
        /* checa roda: A,5,4,3,2 */
        int temA = 0, temK = 0;
        for (int i = 0; i < 5; i++) if (valores[i] == 14) temA = 1;
        if (temA) {
            int v2[5];
            for (int i = 0; i < 5; i++) v2[i] = (valores[i] == 14) ? 1 : valores[i];
            ordenarDecrescente(v2, 5);
            int ok = 1;
            for (int i = 0; i < 4; i++) if (v2[i] - v2[i + 1] != 1) ok = 0;
            if (ok) { seq = 1; memcpy(valoresSeq, v2, sizeof(v2)); }
        }
        (void)temK;
    }

    /* Conta repetições de cada valor */
    int contagem[15] = {0};
    for (int i = 0; i < 5; i++) contagem[valores[i]]++;

    int pares = 0, trincas = 0, quadras = 0;
    int valorTrinca = 0, valorQuadra = 0;
    int valoresPares[2] = {0, 0};
    int idxPar = 0;
    for (int v = 14; v >= 2; v--) {
        if (contagem[v] == 4) { quadras++; valorQuadra = v; }
        else if (contagem[v] == 3) { trincas++; valorTrinca = v; }
        else if (contagem[v] == 2) { pares++; if (idxPar < 2) valoresPares[idxPar++] = v; }
    }

    AvaliacaoMao r;
    memset(r.valoresDesempate, 0, sizeof(r.valoresDesempate));

    if (seq && flush) {
        r.ranking = (valoresSeq[0] == 14) ? ROYAL_FLUSH : STRAIGHT_FLUSH;
        r.valoresDesempate[0] = valoresSeq[0];
    } else if (quadras) {
        r.ranking = QUADRA;
        r.valoresDesempate[0] = valorQuadra;
        for (int i = 0; i < 5; i++) if (valores[i] != valorQuadra) { r.valoresDesempate[1] = valores[i]; break; }
    } else if (trincas && pares) {
        r.ranking = FULL_HOUSE;
        r.valoresDesempate[0] = valorTrinca;
        r.valoresDesempate[1] = valoresPares[0];
    } else if (flush) {
        r.ranking = FLUSH;
        for (int i = 0; i < 5; i++) r.valoresDesempate[i] = valores[i];
    } else if (seq) {
        r.ranking = SEQUENCIA;
        r.valoresDesempate[0] = valoresSeq[0];
    } else if (trincas) {
        r.ranking = TRINCA;
        r.valoresDesempate[0] = valorTrinca;
        int k = 1;
        for (int i = 0; i < 5; i++) if (valores[i] != valorTrinca) r.valoresDesempate[k++] = valores[i];
    } else if (pares == 2) {
        r.ranking = DOIS_PARES;
        r.valoresDesempate[0] = valoresPares[0];
        r.valoresDesempate[1] = valoresPares[1];
        for (int i = 0; i < 5; i++)
            if (valores[i] != valoresPares[0] && valores[i] != valoresPares[1]) { r.valoresDesempate[2] = valores[i]; break; }
    } else if (pares == 1) {
        r.ranking = PAR;
        r.valoresDesempate[0] = valoresPares[0];
        int k = 1;
        for (int i = 0; i < 5; i++) if (valores[i] != valoresPares[0]) r.valoresDesempate[k++] = valores[i];
    } else {
        r.ranking = ALTA_CARTA;
        for (int i = 0; i < 5; i++) r.valoresDesempate[i] = valores[i];
    }
    return r;
}

int compararMaos(AvaliacaoMao a, AvaliacaoMao b) {
    if (a.ranking != b.ranking) return (int)a.ranking - (int)b.ranking;
    for (int i = 0; i < 5; i++) {
        if (a.valoresDesempate[i] != b.valoresDesempate[i])
            return a.valoresDesempate[i] - b.valoresDesempate[i];
    }
    return 0;
}

AvaliacaoMao avaliarMao(Carta cartas[], int numCartas) {
    AvaliacaoMao melhor;
    melhor.ranking = ALTA_CARTA;
    memset(melhor.valoresDesempate, 0, sizeof(melhor.valoresDesempate));
    int primeira = 1;

    if (numCartas < 5) {
        return melhor;
    }

    int idx[5];
    /* Combinações C(numCartas, 5) via índices */
    for (idx[0] = 0; idx[0] < numCartas - 4; idx[0]++)
    for (idx[1] = idx[0] + 1; idx[1] < numCartas - 3; idx[1]++)
    for (idx[2] = idx[1] + 1; idx[2] < numCartas - 2; idx[2]++)
    for (idx[3] = idx[2] + 1; idx[3] < numCartas - 1; idx[3]++)
    for (idx[4] = idx[3] + 1; idx[4] < numCartas; idx[4]++) {
        Carta grupo[5];
        for (int i = 0; i < 5; i++) grupo[i] = cartas[idx[i]];
        AvaliacaoMao atual = avaliar5(grupo);
        if (primeira || compararMaos(atual, melhor) > 0) {
            melhor = atual;
            primeira = 0;
        }
    }
    return melhor;
}

const char *nomeRanking(RankingMao r) {
    static const char *nomes[] = {
        "Carta Alta", "Par", "Dois Pares", "Trinca", "Sequencia",
        "Flush", "Full House", "Quadra", "Straight Flush", "Royal Flush"
    };
    return nomes[r];
}
