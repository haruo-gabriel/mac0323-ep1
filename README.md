*Trabalho redigido por Gabriel Haruo Hanai Takeuchi, NUSP 13671636.*

## Introdução

Este arquivo `README.md` oferece uma documentação para a minha interpretação do EP1 de MAC0323.
Arquivos inclusos no pacote: `main.cpp`, `aerolib.hpp`, `aerolib.cpp`, `README.md`, `README.pdf`.

# Classes e objetos

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

## Listas ligadas auxiliares

As listas ligadas foram usadas para ter maior controle e organização de todos os aviões gerados, visto que não é possível iterar sobre a `std::priority_queue`. Além disso, a separação em listas menores facilita a construção das estatísticas requisitadas no enunciado (tempo médio de espera, quantidade média de combustível, % de acidentes, etc.)

```C++
list<Aviao*> pousosForaQueue;
list<Aviao*> decolagensForaQueue;
list<Aviao*> pousosNaQueue;
list<Aviao*> decolagensNaQueue;
list<Aviao*> avioesMortos;
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

Em primeira instância, uma emergência é definida no momento de geração de um avião com probabilidade `Pe` - nesse caso, `prioridade` é definida como `0`.
Entretanto, é necessário considerar as situações em que o combustível de um avião chega a 0 e quando o tempo de espera ultrapassa 10% do limite. Infelizmente, esse algoritmo **não** lida bem com essas situações.

### A ineficiência do algoritmo, explicada

Como dito anteriormente, o critério da fila é a `prioridade`, cujo valor é estático e determinado na geração do avião. O problema dessa estratégia é que os aviões são objetos dinâmicos - em tese, o combustível diminui e o tempo de voo aumenta ao longo do tempo, e esses detalhes *deveriam* ser considerados. A tentativa foi simular essas movimetações com a propriedade `contador`, em que o combustível restante é `combustivel - contador` e o tempo de espera é `contador`.
Empiricamente, observa-se um acúmulo de pousos que não têm mais combustível ou decolagens que já ultrapassaram seu tempo máximo, mas não conseguem sair da fila pois sua `prioridade` foi gerada como alta.
Exemplo da última simulação com as entradas `T = 20; K = 3; Pp = 0.5; Pe = 0.10; C = 5; V = 5;`:

```
RELATÓRIO DA RODADA 20:
    Pistas: -1 | -2 | -2

    Avião RB596 esperando para pousar com combustível atual -9 e prioridade 5
    Avião IV691 esperando para pousar com combustível atual -8 e prioridade 5
    Avião OB169 esperando para pousar com combustível atual -8 e prioridade 5
    Avião QM035 esperando para pousar com combustível atual -7 e prioridade 5
    Avião ZG510 esperando para pousar com combustível atual -5 e prioridade 5
    Avião MN632 esperando para pousar com combustível atual -2 e prioridade 5
    Avião FK628 esperando para pousar com combustível atual -2 e prioridade 4
    Avião WJ605 esperando para pousar com combustível atual 0 e prioridade 5
    Avião UK237 esperando para pousar com combustível atual 2 e prioridade 5
    Avião TV509 esperando para pousar com combustível atual 1 e prioridade 3
    Avião XC406 esperando para pousar com combustível atual 2 e prioridade 3
    Avião FI059 esperando para decolar com tempo de voo atual 10 e prioridade 5
    Avião SR263 esperando para decolar com tempo de voo atual 7 e prioridade 5
    Avião PM707 esperando para decolar com tempo de voo atual 7 e prioridade 5
    Avião YU735 esperando para decolar com tempo de voo atual 5 e prioridade 4
    Avião UZ096 esperando para decolar com tempo de voo atual 1 e prioridade 4

    Tempo médio de espera (pouso): 0.777778
    Tempo médio de espera (decolagem): 1.92308
    Quantidade média de combustível (esperando pousar): -3.27273
    Quantidade média de combustível (já pousaram): 1.88889
    Quantidade total de aviões gerados: 40
    Quantidade de emergências finalizadas: 5 (0.125%)
    Quantidade de aviões *contabilizados* que caíram: 2 (0.05%)
```

Observe os primeiros aviões já deveriam ter saído da fila e inseridos em `avioesMortos`, mas nunca chegam na raíz do min heap para tomarem `pop`.
Uma alternativa seria reconstruir a heap após toda alteração de dados para os nós serem atualizados - algo que me restou sonhar, pois minhas habilidades de C++ não deram conta por agora.

# Testes

Os testes podem ser reproduzidos mudando a seed em `aerolib.hpp` e as entradas em `main.cpp` (comentando ou descomentando as linhas certas para mudar de modo interativo para modo automático):

```C++
// em aerolib.hpp

#define SRAND 1234
```

```C++
// em main.cpp

// modo interativo
// cout << "Digite T, K, Pp, Pe, C e V, respectivamente: ";
// cin >> T >> K >> Pp >> Pe >> C >> V; 

// modo automático 
T = 20; K = 3; Pp = 0.5; Pe = 0.10; C = 5; V = 5;
```

As estatísticas consideradas serão:
- Tempo médio de espera para pousos finalizados
- Tempo médio de espera para decolagens finalizadas
- Quantidade média de combustível para pousos finalizados
- Quantidade total de aviões gerados
- Quantidade de emergências (gerado como emergência, combustível 0, tempo de voo acima do teto) finalizadas
- Quantidade de aviões que caíram

## Teste 01
`T` pequeno, `K` pequeno, `Pp == Pd`, `Pe` pequeno, `C` médio, `V` médio.
```C++
#define SRAND 1234
T = 10; K = 2; Pp = 0.5; Pe = 0.1; C = 5; V = 5;
```

```C++
// output

RELATÓRIO DA RODADA:
    Pistas: -2 | 0 | 1

    Avião QL456 esperando para pousar com combustível atual 0 e prioridade 4
    Avião ST775 esperando para pousar com combustível atual 0 e prioridade 4
    Avião DW649 esperando para pousar com combustível atual 2 e prioridade 3
    Avião UX709 esperando para pousar com combustível atual 4 e prioridade 4
    Avião OK358 esperando para decolar com tempo de voo atual 3 e prioridade 5
    Avião XL152 esperando para decolar com tempo de voo atual 1 e prioridade 4

    Tempo médio de espera (pouso): 0
    Tempo médio de espera (decolagem): 0
    Quantidade média de combustível (esperando pousar): 1.5
    Quantidade média de combustível (já pousaram): 4.5
    Quantidade total de aviões gerados: 14
    Quantidade de emergências finalizadas: 0 (0%)
    Quantidade de aviões *contabilizados* que caíram: 0 (0%)
```

Com esses parâmetros, os pousos e decolagens
