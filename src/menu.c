#include <stdio.h>
#include <stdlib.h>
#include "../include/poker.h"

void exibirMenuPrincipal(void) {
    printf("\n");
    printf("=======================================\n");
    printf("        TEXAS HOLD'EM - MENU           \n");
    printf("=======================================\n");
    printf(" 1. Jogar nova partida\n");
    printf(" 2. Regras do jogo\n");
    printf(" 3. Sair\n");
    printf("=======================================\n");
    printf("Escolha uma opcao: ");
}

int lerOpcaoMenu(void) {
    int opc;
    if (scanf("%d", &opc) != 1) {
        while (getchar() != '\n'); /* limpa buffer invalido */
        return -1;
    }
    return opc;
}

void exibirRegras(void) {
    printf("\n----- REGRAS BASICAS DO TEXAS HOLD'EM -----\n");
    printf("1. Cada jogador recebe 2 cartas privadas (hole cards).\n");
    printf("2. Cinco cartas comunitarias sao reveladas em etapas:\n");
    printf("   - Flop (3 cartas)\n");
    printf("   - Turn (1 carta)\n");
    printf("   - River (1 carta)\n");
    printf("3. Ha rodadas de apostas apos as cartas privadas, apos o\n");
    printf("   flop, apos o turn e apos o river.\n");
    printf("4. O jogador forma a melhor mao de 5 cartas combinando\n");
    printf("   suas 2 cartas privadas com as 5 comunitarias.\n");
    printf("5. Ranking de maos (da mais fraca a mais forte):\n");
    printf("   Carta Alta < Par < Dois Pares < Trinca < Sequencia\n");
    printf("   < Flush < Full House < Quadra < Straight Flush < Royal Flush\n");
    printf("6. Quem tiver a melhor mao no showdown vence o pote.\n");
    printf("---------------------------------------------\n");
    printf("Pressione ENTER para voltar ao menu...");
    while (getchar() != '\n');
    getchar();
}

void telaConfiguracaoJogo(void) {
    printf("\nConfiguracao rapida: voce jogara contra bots.\n");
}
