#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/poker.h"

void inicializarBaralho(Baralho *b) {
    int idx = 0;
    for (int naipe = 0; naipe < NUM_PALOS; naipe++) {
        for (int valor = 2; valor <= 14; valor++) {
            b->cartas[idx].valor = valor;
            b->cartas[idx].naipe = naipe;
            idx++;
        }
    }
    b->topo = 0;
}

void embaralhar(Baralho *b) {
    for (int i = TAMANHO_BARALHO - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Carta tmp = b->cartas[i];
        b->cartas[i] = b->cartas[j];
        b->cartas[j] = tmp;
    }
    b->topo = 0;
}

Carta comprarCarta(Baralho *b) {
    if (b->topo >= TAMANHO_BARALHO) {
        /* Segurança: reembaralha se acabar (não deve ocorrer em uma mão normal) */
        embaralhar(b);
    }
    return b->cartas[b->topo++];
}

const char *nomeValor(int valor) {
    static const char *nomes[] = {
        "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
    };
    return nomes[valor - 2];
}

const char *nomeNaipe(int naipe) {
    static const char *nomes[] = {"Copas", "Ouros", "Paus", "Espadas"};
    return nomes[naipe];
}

void imprimirCarta(Carta c) {
    printf("[%s de %s]", nomeValor(c.valor), nomeNaipe(c.naipe));
}
