# ♠️ Texas Hold'em em C

![Build](https://github.com/Ronaldolms/Texas-Holdem-/actions/workflows/ci.yml/badge.svg)
![Language](https://img.shields.io/badge/language-C-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

Jogo de poker **Texas Hold'em** em C puro, com menu interativo no terminal, jogado contra bots.

## 🎮 Demonstração
```
=======================================
        TEXAS HOLD'EM - MENU           
=======================================
 1. Jogar nova partida
 2. Regras do jogo
 3. Sair
=======================================
Escolha uma opcao: 
```


## ✨ Funcionalidades

- Menu principal (jogar / ver regras / sair)
- Suporte de 2 a 6 jogadores (você + bots)
- Baralho completo com embaralhamento (Fisher-Yates)
- Rodadas de apostas: pré-flop, flop, turn, river
- Blinds automáticos (small/big blind) com dealer rotativo
- Avaliador de mãos completo (par até Royal Flush), incluindo comparação de desempate
- Bots com decisões simples de fold/call/raise
- Showdown com exibição das mãos e distribuição do pote

## 📁 Estrutura do projeto
texas-holdem/
├── include/
│ └── poker.h # structs e protótipos
├── src/
│ ├── main.c # loop principal / fluxo da partida
│ ├── menu.c # menu e telas de texto
│ ├── baralho.c # baralho, embaralhar, comprar carta
│ ├── jogo.c # regras da mesa, apostas, distribuição do pote
│ └── avaliador.c # avaliação e ranking de mãos de poker
├── .github/
│ └── workflows/
│ └── ci.yml # build automático a cada push/PR
├── Makefile
├── LICENSE
└── README.md

## 🔧 Como compilar

Requer `gcc` e `make`.

```bash
make
```

Isso gera o executável `texas_holdem` na raiz do projeto.

## ▶️ Como jogar

```bash
./texas_holdem
```

Siga o menu:

1. **Jogar nova partida** — escolha quantos jogadores (você + bots) e jogue as rodadas de apostas.
2. **Regras do jogo** — mostra um resumo das regras do Texas Hold'em.
3. **Sair**

Durante a rodada de apostas, você escolhe entre:

| Opção | Ação |
|-------|------|
| `1` | Pagar (call) ou Passar (check), se não houver nada a pagar |
| `2` | Aumentar a aposta (raise) |
| `3` | Desistir (fold) |

## 🧹 Limpar build

```bash
make clean
```

## 🗺️ Possíveis melhorias futuras

- [ ] Side pots para all-ins com valores diferentes
- [ ] IA de bots mais sofisticada (baseada em força real da mão)
- [ ] Suporte a múltiplas mesas / torneio
- [ ] Interface gráfica (ncurses ou similar)
- [ ] Testes unitários para o avaliador de mãos

## 🤝 Contribuindo

Contribuições são bem-vindas! Sinta-se livre para abrir uma *issue* ou um *pull request*.

## 📄 Licença

Este projeto está licenciado sob a licença MIT — veja o arquivo [LICENSE](LICENSE) para mais detalhes.