# Texas Hold'em em C

Jogo de poker Texas Hold'em em C puro, com menu interativo no terminal, jogado contra bots.

## Funcionalidades

- Menu principal (jogar / ver regras / sair)
- Suporte de 2 a 6 jogadores (você + bots)
- Baralho completo com embaralhamento (Fisher-Yates)
- Rodadas de apostas: pré-flop, flop, turn, river
- Blinds automáticos (small/big blind) com dealer rotativo
- Avaliador de mãos completo (par até Royal Flush), incluindo comparação de desempate
- Bots com decisões simples de fold/call/raise
- Showdown com exibição das mãos e distribuição do pote

## Estrutura do projeto

```
texas-holdem/
├── include/
│   └── poker.h        # structs e protótipos
├── src/
│   ├── main.c          # loop principal / fluxo da partida
│   ├── menu.c           # menu e telas de texto
│   ├── baralho.c         # baralho, embaralhar, comprar carta
│   ├── jogo.c              # regras da mesa, apostas, distribuição do pote
│   └── avaliador.c          # avaliação e ranking de mãos de poker
├── Makefile
└── README.md
```

## Como compilar

Requer `gcc` e `make`.

```bash
make
```

Isso gera o executável `texas_holdem` na raiz do projeto.

## Como jogar

```bash
./texas_holdem
```

Siga o menu:

1. **Jogar nova partida** — escolha quantos jogadores (você + bots) e jogue as rodadas de apostas.
2. **Regras do jogo** — mostra um resumo das regras do Texas Hold'em.
3. **Sair**

Durante a rodada de apostas, você escolhe entre:

- `1` — Pagar (call) ou Passar (check), se não houver nada a pagar
- `2` — Aumentar a aposta (raise)
- `3` — Desistir (fold)

## Limpar build

```bash
make clean
```

## Possíveis melhorias futuras

- Side pots para all-ins com valores diferentes
- IA de bots mais sofisticada (baseada em força real da mão)
- Suporte a múltiplas mesas / torneio
- Interface gráfica (ncurses ou similar)

## Licença

Sinta-se livre para usar, modificar e distribuir este projeto.
