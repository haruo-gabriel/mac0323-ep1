*Trabalho redigido por Gabriel Haruo Hanai Takeuchi, NUSP 13671636.*

# Introdução

Este arquivo `README.md` oferece uma documentação para a minha interpretação do EP1 de MAC0323.

# Classes

# Estruturas de dados usadas

## A fila de prioridade

### Definição de 'emergência'

### Prioridade de um avião

## Listas ligadas auxiliares

A propriedade `int prioridade` da classe `Aviao` toma o valor do combustível (caso seja um pouso) ou o valor do tempo máximo de voo (caso seja uma decolagem). Dessa forma, os aviões que tiverem o menor valor absoluto de prioridade serão tratados com mais urgência. Isso evita que as propriedades dos nós da fila de prioridade sejam alterados, visto que filas atualizam apenas com um `pop` ou `push`.

# O algoritmo

# Testes