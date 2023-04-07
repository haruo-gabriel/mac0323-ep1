*Trabalho redigido por Gabriel Haruo Hanai Takeuchi, NUSP 13671636.*

# Introdução

Este arquivo `README.md` oferece uma documentação para a minha interpretação do EP1 de MAC0323.

# Classes

Duas classes foram usadas: `class Pista` e `class Aviao`, definidas a seguir:
```C++
class Pista {
public:
    int livre;

    Pista();

    void cooldown();
};
```

*Observação:*
Uma pista livre assume o valor `livre == 1`. Quando recém-ocupada, assume o valor `livre == -2`. É importante ressaltar que, para simular o cooldown das pistas, `livre` é incrementado a cada rodada até assumir novamente o valor `livre == 1`.


```C++
class Aviao {
public:
    string id, codigo;
    int prioridade, contador, combustivel, tempoVoo;
    // se pouso, tempoVoo = -1
    // se decolagem, combustivel = -1
    bool ehPouso;

    Aviao(float Pp, float Pe, bool ehPouso, int C, int V);

    string setId();
    string setCodigo(bool ehPouso);
    int setCombustivel(int C);
    int setTempoVoo(int V);
    int setPrioridade(float Pe, int C, int V);
};
```
*Observação:*
A propriedade `contador` contabiliza quanto tempo um avião está na fila de prioridade.
A propriedade `prioridade` será explicada em breve.


# Estruturas de dados usadas

Foram usadas, no total, 6 estruturas de dados:
- uma fila de prioridade
- uma lista ligada para monitorar os aviões que ainda não pousaram  
- uma lista ligada para monitorar os aviões que ainda não decolaram
- uma lista ligada para monitorar os aviões que já pousaram 
- uma lista ligada para monitorar os aviões que já decolaram
- uma lista ligada para monitorar os pousos que caíram
- um vetor para armazenar as pistas de voo

```C++
    priority_queue<Aviao*, vector<Aviao*>, decltype(comparaAviao)> queue(comparaAviao);
    list<Aviao*> pousosNaQueue;
    list<Aviao*> decolagensNaQueue;
    list<Aviao*> pousosForaQueue;
    list<Aviao*> decolagensForaQueue;
    list<Aviao*> avioesMortos;
    vector<Pista> pistas;
```

## A fila de prioridade

```C++
    priority_queue<Aviao*, vector<Aviao*>, decltype(comparaAviao)> queue(comparaAviao);
```

Como requisitado no enunciado, a estrutura `std::priority_queue` customizada armazena ponteiros para os objetos `Aviao*`. Um detalhe importante é o comparador `comparaAviao`, o qual efetivamente torna a fila em um min heap de acordo com a propriedade  `prioridade` dos aviões.

### Prioridade de um avião

A propriedade `int prioridade` da classe `Aviao` toma

- valor `0` caso o avião seja gerado como uma emergência
- valor do combustível caso seja um pouso
- valor do tempo máximo de voo caso seja uma decolagem

Dessa forma, os aviões que tiverem o menor valor absoluto de prioridade serão tratados com mais urgência.
A prioridade é determinada na geração de um avião, e permanece estática ao longo do algoritmo. Por um lado, 

Isso evita que  as propriedades dos nós da fila de prioridade sejam alterados, visto que filas atualizam apenas com um `pop` ou `push`.

### Definição de 'emergência'

Essa sessão possui, sem dúvida, o maior caráter subjetivo e problemático do texto.

Em primeira instância, uma emergência é definida no momento de geração de um avião. Com probabilidade


## Listas ligadas auxiliares

# O algoritmo

# Testes