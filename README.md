*Trabalho redigido por Gabriel Haruo Hanai Takeuchi, NUSP 13671636.*

# Introdução

Este arquivo `README.md` oferece uma documentação para a minha interpretação do EP1 de MAC0323.

# Classes

# A fila de prioridade

- vector
- priority_queue

## Definição de 'emergência'


## Prioridade de um avião

A propriedade `int prioridade` da classe `Aviao` toma um valor de 0 a 4, sendo 4 a prioridade máxima. O critério para a escala de prioridade segue o modelo a seguir:

0. Pouso/Decolagem
1. Decolagem que ultrapassou 10% do tempo de voo
2. Pouso com combustível em 0
3. Pouso ou decolagem de emergência

A propriedade `int prioridade` da classe `Aviao` toma o valor do combustível (caso seja um pouso) ou o valor do tempo máximo de voo (caso seja uma decolagem). Dessa forma, os aviões que tiverem o menor valor absoluto de prioridade serão tratados com mais urgência. Isso evita que as propriedades dos nós da fila de prioridade sejam alterados, visto que filas atualizam apenas com um `pop` ou `push`.

# O algoritmo