#ifndef POKER_H
#define POKER_H

#define NUM_PALOS 4
#define NUM_VALORES 13
#define MAX_JOGADORES 6
#define CARTAS_MAO 2
#define MAX_COMUNITARIAS 5
#define TAMANHO_BARALHO 52
#define STACK_INICIAL 1000
#define SMALL_BLIND 10
#define BIG_BLIND 20

typedef struct {
    int valor; /* 2-14 (11=J,12=Q,13=K,14=A) */
    int naipe; /* 0=Copas,1=Ouros,2=Paus,3=Espadas */
} Carta;

typedef struct {
    Carta cartas[TAMANHO_BARALHO];
    int topo;
} Baralho;

typedef struct {
    char nome[30];
    Carta mao[CARTAS_MAO];
    int stack;
    int apostaRodada;
    int ativo;      /* 1 = ainda na mao, 0 = deu fold */
    int allIn;
    int ehBot;
} Jogador;

typedef struct {
    Jogador jogadores[MAX_JOGADORES];
    int numJogadores;
    Baralho baralho;
    Carta comunitarias[MAX_COMUNITARIAS];
    int numComunitarias;
    int pote;
    int dealerPos;
    int mesaAtiva;
} Jogo;

typedef enum {
    ALTA_CARTA = 0,
    PAR,
    DOIS_PARES,
    TRINCA,
    SEQUENCIA,
    FLUSH,
    FULL_HOUSE,
    QUADRA,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
} RankingMao;

typedef struct {
    RankingMao ranking;
    int valoresDesempate[5];
} AvaliacaoMao;

/* baralho.c */
void inicializarBaralho(Baralho *b);
void embaralhar(Baralho *b);
Carta comprarCarta(Baralho *b);
const char *nomeValor(int valor);
const char *nomeNaipe(int naipe);
void imprimirCarta(Carta c);

/* jogo.c */
void inicializarJogo(Jogo *j, int numJogadores, int numBots);
void iniciarNovaMao(Jogo *j);
void rodadaApostas(Jogo *j, int cartasComunitariasReveladas);
void distribuirCartasComunitarias(Jogo *j, int quantidade);
void mostrarMesa(Jogo *j);
int contarJogadoresAtivos(Jogo *j);
void distribuirPote(Jogo *j);
int jogoTerminou(Jogo *j);

/* avaliador.c */
AvaliacaoMao avaliarMao(Carta cartas[], int numCartas);
int compararMaos(AvaliacaoMao a, AvaliacaoMao b); /* >0 se a>b, <0 se a<b, 0 empate */
const char *nomeRanking(RankingMao r);

/* menu.c */
void exibirMenuPrincipal(void);
void exibirRegras(void);
int lerOpcaoMenu(void);
void telaConfiguracaoJogo(void);

#endif
