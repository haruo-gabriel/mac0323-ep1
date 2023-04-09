*Trabalho redigido por Gabriel Haruo Hanai Takeuchi, de NUSP 13671636.*

# Introdução

Este arquivo `README.md` oferece uma documentação para a minha interpretação do EP1 de MAC0323.
Arquivos inclusos no pacote: `main.cpp`, `aerolib.hpp`, `aerolib.cpp`, `README.md`, `README.pdf`.

### Como compilar e executar

```bash
clear && g++ -Wall -std=c++11 *.cpp -o a.out && ./a.out
```

Os parâmetros podem ser modificados de acordo com a primeira parte da sessão [Testes](#testes)

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
A propriedade `contador` contabiliza quanto tempo um avião está na fila de prioridade. Útil para calcular o combustível restante de um avião ou o seu tempo de voo.
A propriedade `prioridade` será explicada em breve [nesta sessão](#prioridade-de-um-avião).


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
A `prioridade` é determinada na geração de um avião, e permanece estática ao longo do algoritmo. Nos próximos tópicos, será explicado o motivo de `prioridade` ser estático e não dinâmico, como era desejado.


### Definição de 'emergência'

Essa sessão possui, sem dúvida, o maior caráter subjetivo e problemático do texto.

Em primeira instância, uma emergência é definida no momento de geração de um avião com probabilidade `Pe` - nesse caso, `prioridade` é definida como `0`.
Entretanto, é necessário considerar as situações em que o combustível de um avião chega a 0 e quando o tempo de espera ultrapassa 10% do limite. Infelizmente, esse algoritmo **não** lida bem com essas situações.

### A ineficiência do algoritmo, explicada

Como dito anteriormente, o critério da fila é a `prioridade`, cujo valor é estático e determinado na geração do avião. O problema dessa estratégia é que os aviões são objetos dinâmicos - em tese, o combustível diminui e o tempo de voo aumenta, e esses detalhes *deveriam* ser considerados.
O grande problema encontrado foi que, mesmo se a `prioridade` dos objetos aviões fossem atualizadas à medida em que o combustível acaba ou o tempo de voo aumenta, a fila **não reposiciona automaticamente** seus nós, ou seja, ela não 'heapfica' automaticamente. Uma alternativa seria reconstruir a heap após toda alteração de dados para os nós serem atualizados - algo que me restou sonhar, pois minhas habilidades de C++ não deram conta por agora.
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

# Testes

Os testes podem ser reproduzidos mudando a seed e as entradas em `main.cpp` (comentando ou descomentando as linhas certas para mudar de modo interativo para modo automático):


```C++
// em main.cpp

int seed = 123;

// modo interativo
// cout << "Digite T, K, Pp, Pe, C e V, respectivamente: ";
// cin >> T >> K >> Pp >> Pe >> C >> V; 

// modo automático
T = 10; K = 2; Pp = 0.5; Pe = 0.1; C = 5; V = 5;
```

Todos os exemplos de testes são da última rodada, em que as estatísticas estão completas. As estatísticas consideradas foram:
1. Tempo médio de espera para pousos finalizados
2. Tempo médio de espera para decolagens finalizadas
3. Quantidade média de combustível para pousos finalizados
4. Quantidade total de aviões gerados
5. Quantidade de emergências (gerado como emergência, combustível 0, tempo de voo acima do teto) finalizadas
6. Quantidade de aviões *contabilizados* que caíram


### Teste 01
`T` pequeno, `K` pequeno, `Pp == Pd`, `Pe` pequeno, `C` médio, `V` médio.
```C++
T = 10; K = 2; Pp = 0.5; Pe = 0.1; C = 5; V = 5;
```

```
int seed = 123;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | -2 | 0

Avião BU445 esperando para pousar com combustível atual 0 e prioridade 4
Avião IB418 esperando para pousar com combustível atual 4 e prioridade 5
Avião HA481 esperando para decolar com tempo de voo atual 7 e prioridade 5
Avião DN325 esperando para decolar com tempo de voo atual 3 e prioridade 3

Tempo médio de espera (pousos): 0.4
Tempo médio de espera (decolagens): 0.166667
Quantidade média de combustível (esperando pousar): 2
Quantidade média de combustível (já pousaram): 3.2
Quantidade total de aviões gerados: 16
Quantidade de emergências finalizadas: 1 (6.25%)
Quantidade de aviões *contabilizados* que caíram: 0 (0%)
```

```
int seed = 456;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | -2 | 0

Avião UD881 esperando para pousar com combustível atual 1 e prioridade 5
Avião GN788 esperando para pousar com combustível atual 2 e prioridade 5
Avião IT652 esperando para pousar com combustível atual 5 e prioridade 5

Tempo médio de espera (pousos): 0.5
Tempo médio de espera (decolagens): 1
Quantidade média de combustível (esperando pousar): 2.66667
Quantidade média de combustível (já pousaram): 1.33333
Quantidade total de aviões gerados: 15
Quantidade de emergências finalizadas: 1 (6.66667%)
Quantidade de aviões *contabilizados* que caíram: 0 (0%)
```

```
int seed = 789;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | -2 | -1

Avião VN004 esperando para pousar com combustível atual 0 e prioridade 5
Avião XO411 esperando para decolar com tempo de voo atual 8 e prioridade 5
Avião ZJ822 esperando para decolar com tempo de voo atual 1 e prioridade 5
Avião DU700 esperando para decolar com tempo de voo atual 0 e prioridade 5
Avião PX142 esperando para decolar com tempo de voo atual 0 e prioridade 5

Tempo médio de espera (pousos): 2.75
Tempo médio de espera (decolagens): 0.333333
Quantidade média de combustível (esperando pousar): 0
Quantidade média de combustível (já pousaram): 0.25
Quantidade total de aviões gerados: 17
Quantidade de emergências finalizadas: 2 (11.7647%)
Quantidade de aviões *contabilizados* que caíram: 0 (0%)
```

Com esses parâmetros, observa-se que o algoritmo é capaz de lidar com as demandas com certa eficiência:
- Em média, 1.216 tempos para pouso e 0.5 tempos para decolagem
- Relativa baixa taxa de aviões colocados em situação de emergência (em média 8.227% do total de voos)
- Aparente baixa taxa de quedas (em média, 0 aviões caíram), mas é possível observar que há aviões com combustível atual 0 e outros com tempo de voo além do limite, os quais não foram contabilizados. É a falha do algoritmo que mencionei nas sessões anteriores.


### Teste 02
`T` grande, `K` pequeno, `Pp == Pd`, `Pe` pequeno, `C` médio, `V` médio.
```C++
T = 1000; K = 2; Pp = 0.5; Pe = 0.1; C = 5; V = 5;
```

```
int seed = 123;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | -2 | -1

Avião CU739 esperando para pousar com combustível atual -3 e prioridade 5
Avião HF113 esperando para pousar com combustível atual -2 e prioridade 5
Avião SS122 esperando para pousar com combustível atual -1 e prioridade 5
Avião KA932 esperando para pousar com combustível atual 0 e prioridade 5
Avião SO219 esperando para pousar com combustível atual 3 e prioridade 4
Avião GJ930 esperando para pousar com combustível atual 5 e prioridade 5
Avião MT057 esperando para decolar com tempo de voo atual 953 e prioridade 0
Avião ZS681 esperando para decolar com tempo de voo atual 947 e prioridade 0
Avião MY955 esperando para decolar com tempo de voo atual 910 e prioridade 0
Avião RN090 esperando para decolar com tempo de voo atual 842 e prioridade 0
Avião YX956 esperando para decolar com tempo de voo atual 748 e prioridade 0
Avião RO839 esperando para decolar com tempo de voo atual 653 e prioridade 0
Avião KW760 esperando para decolar com tempo de voo atual 598 e prioridade 0
Avião PE018 esperando para decolar com tempo de voo atual 574 e prioridade 0
Avião LA683 esperando para decolar com tempo de voo atual 503 e prioridade 0
Avião DX381 esperando para decolar com tempo de voo atual 457 e prioridade 0
Avião LJ318 esperando para decolar com tempo de voo atual 419 e prioridade 0
Avião SK523 esperando para decolar com tempo de voo atual 395 e prioridade 0
Avião RC885 esperando para decolar com tempo de voo atual 350 e prioridade 0
Avião KZ284 esperando para decolar com tempo de voo atual 319 e prioridade 0
Avião VO960 esperando para decolar com tempo de voo atual 301 e prioridade 0
Avião TY284 esperando para decolar com tempo de voo atual 286 e prioridade 0
Avião FG563 esperando para decolar com tempo de voo atual 278 e prioridade 0
Avião ZS140 esperando para decolar com tempo de voo atual 253 e prioridade 0
Avião DY112 esperando para decolar com tempo de voo atual 247 e prioridade 0
Avião XI752 esperando para decolar com tempo de voo atual 227 e prioridade 0
Avião OU363 esperando para decolar com tempo de voo atual 223 e prioridade 0
Avião RW405 esperando para decolar com tempo de voo atual 203 e prioridade 0
Avião GG202 esperando para decolar com tempo de voo atual 193 e prioridade 0
Avião MC211 esperando para decolar com tempo de voo atual 167 e prioridade 0
Avião TQ168 esperando para decolar com tempo de voo atual 115 e prioridade 0
Avião RQ428 esperando para decolar com tempo de voo atual 83 e prioridade 0
Avião CD451 esperando para decolar com tempo de voo atual 34 e prioridade 0
Avião XC097 esperando para decolar com tempo de voo atual 2 e prioridade 5

Tempo médio de espera (pousos): 2.62757
Tempo médio de espera (decolagens): 7.09409
Quantidade média de combustível (esperando pousar): 0.333333
Quantidade média de combustível (já pousaram): 0.926686
Quantidade total de aviões gerados: 1506
Quantidade de emergências finalizadas: 322 (21.3811%)
Quantidade de aviões *contabilizados* que caíram: 206 (13.6786%)
```

```
int seed = 456;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | -2 | -2

Avião OR590 esperando para pousar com combustível atual -38 e prioridade 5
Avião QG308 esperando para pousar com combustível atual -17 e prioridade 5
Avião PT826 esperando para pousar com combustível atual -14 e prioridade 5
Avião AZ389 esperando para pousar com combustível atual -10 e prioridade 5
Avião PH526 esperando para pousar com combustível atual -7 e prioridade 5
Avião ZD502 esperando para pousar com combustível atual 2 e prioridade 4
Avião BI687 esperando para pousar com combustível atual 4 e prioridade 5
Avião ZH527 esperando para decolar com tempo de voo atual 967 e prioridade 0
Avião LS468 esperando para decolar com tempo de voo atual 958 e prioridade 0
Avião YV044 esperando para decolar com tempo de voo atual 917 e prioridade 0
Avião XU983 esperando para decolar com tempo de voo atual 782 e prioridade 0
Avião ST141 esperando para decolar com tempo de voo atual 781 e prioridade 0
Avião XR588 esperando para decolar com tempo de voo atual 680 e prioridade 0
Avião SE130 esperando para decolar com tempo de voo atual 626 e prioridade 0
Avião UF720 esperando para decolar com tempo de voo atual 622 e prioridade 0
Avião KZ536 esperando para decolar com tempo de voo atual 586 e prioridade 0
Avião IE265 esperando para decolar com tempo de voo atual 559 e prioridade 0
Avião CE462 esperando para decolar com tempo de voo atual 550 e prioridade 0
Avião ZO191 esperando para decolar com tempo de voo atual 538 e prioridade 0
Avião AX714 esperando para decolar com tempo de voo atual 490 e prioridade 0
Avião TZ356 esperando para decolar com tempo de voo atual 446 e prioridade 0
Avião FU425 esperando para decolar com tempo de voo atual 430 e prioridade 0
Avião PL802 esperando para decolar com tempo de voo atual 430 e prioridade 0
Avião RD539 esperando para decolar com tempo de voo atual 391 e prioridade 0
Avião LI201 esperando para decolar com tempo de voo atual 362 e prioridade 0
Avião LM085 esperando para decolar com tempo de voo atual 242 e prioridade 0
Avião PW548 esperando para decolar com tempo de voo atual 226 e prioridade 0
Avião LT100 esperando para decolar com tempo de voo atual 97 e prioridade 0
Avião CG723 esperando para decolar com tempo de voo atual 46 e prioridade 0
Avião DW776 esperando para decolar com tempo de voo atual 28 e prioridade 0
Avião JK925 esperando para decolar com tempo de voo atual 21 e prioridade 5
Avião TZ316 esperando para decolar com tempo de voo atual 13 e prioridade 4
Avião LL517 esperando para decolar com tempo de voo atual 9 e prioridade 4
Avião RC267 esperando para decolar com tempo de voo atual 3 e prioridade 5

Tempo médio de espera (pousos): 2.16209
Tempo médio de espera (decolagens): 7.25758
Quantidade média de combustível (esperando pousar): -11.4286
Quantidade média de combustível (já pousaram): 1.17308
Quantidade total de aviões gerados: 1498
Quantidade de emergências finalizadas: 321 (21.4286%)
Quantidade de aviões *contabilizados* que caíram: 193 (12.8838%)
```

```
int seed = 789;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | 0 | 0

Avião OS217 esperando para pousar com combustível atual -25 e prioridade 5
Avião IF403 esperando para pousar com combustível atual -18 e prioridade 5
Avião LR210 esperando para pousar com combustível atual -11 e prioridade 5
Avião XI112 esperando para pousar com combustível atual -7 e prioridade 4
Avião RP821 esperando para pousar com combustível atual 1 e prioridade 3
Avião LY699 esperando para pousar com combustível atual 1 e prioridade 1
Avião XQ882 esperando para decolar com tempo de voo atual 887 e prioridade 0
Avião EQ281 esperando para decolar com tempo de voo atual 854 e prioridade 0
Avião NZ647 esperando para decolar com tempo de voo atual 817 e prioridade 0
Avião AO655 esperando para decolar com tempo de voo atual 766 e prioridade 0
Avião XK830 esperando para decolar com tempo de voo atual 752 e prioridade 0
Avião PE519 esperando para decolar com tempo de voo atual 730 e prioridade 0
Avião NP347 esperando para decolar com tempo de voo atual 728 e prioridade 0
Avião JZ819 esperando para decolar com tempo de voo atual 709 e prioridade 0
Avião UY536 esperando para decolar com tempo de voo atual 653 e prioridade 0
Avião QS132 esperando para decolar com tempo de voo atual 652 e prioridade 0
Avião KA363 esperando para decolar com tempo de voo atual 637 e prioridade 0
Avião MO462 esperando para decolar com tempo de voo atual 563 e prioridade 0
Avião JY632 esperando para decolar com tempo de voo atual 550 e prioridade 0
Avião FH967 esperando para decolar com tempo de voo atual 542 e prioridade 0
Avião GE067 esperando para decolar com tempo de voo atual 539 e prioridade 0
Avião LB652 esperando para decolar com tempo de voo atual 538 e prioridade 0
Avião OI665 esperando para decolar com tempo de voo atual 494 e prioridade 0
Avião XC702 esperando para decolar com tempo de voo atual 475 e prioridade 0
Avião UZ607 esperando para decolar com tempo de voo atual 455 e prioridade 0
Avião FM461 esperando para decolar com tempo de voo atual 431 e prioridade 0
Avião VH036 esperando para decolar com tempo de voo atual 383 e prioridade 0
Avião SR873 esperando para decolar com tempo de voo atual 379 e prioridade 0
Avião KY999 esperando para decolar com tempo de voo atual 347 e prioridade 0
Avião YI695 esperando para decolar com tempo de voo atual 310 e prioridade 0
Avião XC558 esperando para decolar com tempo de voo atual 263 e prioridade 0
Avião GD541 esperando para decolar com tempo de voo atual 244 e prioridade 0
Avião IS653 esperando para decolar com tempo de voo atual 238 e prioridade 0
Avião RH481 esperando para decolar com tempo de voo atual 158 e prioridade 0
Avião TJ903 esperando para decolar com tempo de voo atual 152 e prioridade 0
Avião TR358 esperando para decolar com tempo de voo atual 118 e prioridade 0
Avião MT215 esperando para decolar com tempo de voo atual 91 e prioridade 0
Avião DH122 esperando para decolar com tempo de voo atual 56 e prioridade 0
Avião QP391 esperando para decolar com tempo de voo atual 55 e prioridade 0
Avião NV169 esperando para decolar com tempo de voo atual 40 e prioridade 0
Avião QU660 esperando para decolar com tempo de voo atual 24 e prioridade 5
Avião ST722 esperando para decolar com tempo de voo atual 18 e prioridade 5
Avião PP480 esperando para decolar com tempo de voo atual 17 e prioridade 5
Avião ZL142 esperando para decolar com tempo de voo atual 6 e prioridade 5
Avião ND211 esperando para decolar com tempo de voo atual 6 e prioridade 4
Avião QP788 esperando para decolar com tempo de voo atual 4 e prioridade 5
Avião CN021 esperando para decolar com tempo de voo atual 2 e prioridade 2
Avião NW121 esperando para decolar com tempo de voo atual 1 e prioridade 4
Avião GE523 esperando para decolar com tempo de voo atual 1 e prioridade 1

Tempo médio de espera (pousos): 2.41667
Tempo médio de espera (decolagens): 5.33333
Quantidade média de combustível (esperando pousar): -9.83333
Quantidade média de combustível (já pousaram): 0.844828
Quantidade total de aviões gerados: 1491
Quantidade de emergências finalizadas: 300 (20.1207%)
Quantidade de aviões *contabilizados* que caíram: 197 (13.2126%)
```

Com um `T` grande, observa-se que o algoritmo denuncia sua ineficiência:
- Em média, 2.40211 tempos para pouso (aumento de 97,5% em relação ao teste 01) e 6.5616 tempos para decolagem (aumento de 13 vezes em relação ao teste 01)
- Alta taxa de emergências finalizadas (em média, 20.9768%)
- Alta taxa de aviões caídos (em média, 13.2583%), além de todos os aviões que ficaram presos na fila de prioridade (pode ser percebido com a quantidade média de combustível negativa dos aviões que ainda não pousaram)


### Teste 03
`T` pequeno, `K` grande, `Pp == Pd`, `Pe` pequeno, `C` médio, `V` médio.
```C++
T = 10; K = 6; Pp = 0.5; Pe = 0.1; C = 5; V = 5;
```

```
int seed = 123;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | -2 | 0

Avião WI681 esperando para pousar com combustível atual -2 e prioridade 3
Avião ED547 esperando para pousar com combustível atual 0 e prioridade 4
Avião PA555 esperando para pousar com combustível atual 1 e prioridade 4
Avião DH054 esperando para pousar com combustível atual 2 e prioridade 4
Avião WC532 esperando para pousar com combustível atual 1 e prioridade 2
Avião CW711 esperando para pousar com combustível atual 2 e prioridade 3
Avião HP532 esperando para pousar com combustível atual 3 e prioridade 4
Avião PT821 esperando para pousar com combustível atual 5 e prioridade 5
Avião HA481 esperando para decolar com tempo de voo atual 7 e prioridade 5
Avião XK860 esperando para decolar com tempo de voo atual 5 e prioridade 4
Avião VL933 esperando para decolar com tempo de voo atual 5 e prioridade 5
Avião YR187 esperando para decolar com tempo de voo atual 4 e prioridade 4
Avião SP841 esperando para decolar com tempo de voo atual 3 e prioridade 4
Avião GO619 esperando para decolar com tempo de voo atual 3 e prioridade 5
Avião KR424 esperando para decolar com tempo de voo atual 3 e prioridade 3
Avião TG278 esperando para decolar com tempo de voo atual 2 e prioridade 4
Avião VP149 esperando para decolar com tempo de voo atual 1 e prioridade 3
Avião XJ787 esperando para decolar com tempo de voo atual 1 e prioridade 5
Avião RI694 esperando para decolar com tempo de voo atual 0 e prioridade 1
Avião CK458 esperando para decolar com tempo de voo atual 0 e prioridade 2

Tempo médio de espera (pousos): 0.5
Tempo médio de espera (decolagens): 0.4
Quantidade média de combustível (esperando pousar): 1.5
Quantidade média de combustível (já pousaram): 2.66667
Quantidade total de aviões gerados: 33
Quantidade de emergências finalizadas: 2 (6.06061%)
Quantidade de aviões *contabilizados* que caíram: 0 (0%)
```

```
int seed = 456;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | -2 | -2

Avião ZS734 esperando para pousar com combustível atual -4 e prioridade 4
Avião HI163 esperando para pousar com combustível atual 1 e prioridade 4
Avião DG043 esperando para pousar com combustível atual 1 e prioridade 3
Avião EO664 esperando para pousar com combustível atual 4 e prioridade 4
Avião FV160 esperando para decolar com tempo de voo atual 8 e prioridade 4
Avião YE554 esperando para decolar com tempo de voo atual 7 e prioridade 4
Avião MK288 esperando para decolar com tempo de voo atual 7 e prioridade 4
Avião JJ521 esperando para decolar com tempo de voo atual 6 e prioridade 5
Avião EK551 esperando para decolar com tempo de voo atual 6 e prioridade 5
Avião GA855 esperando para decolar com tempo de voo atual 5 e prioridade 0
Avião IS753 esperando para decolar com tempo de voo atual 3 e prioridade 5
Avião AG096 esperando para decolar com tempo de voo atual 3 e prioridade 3
Avião YP239 esperando para decolar com tempo de voo atual 2 e prioridade 4
Avião XH414 esperando para decolar com tempo de voo atual 2 e prioridade 5

Tempo médio de espera (pousos): 1.33333
Tempo médio de espera (decolagens): 2.33333
Quantidade média de combustível (esperando pousar): 0.5
Quantidade média de combustível (já pousaram): 1.33333
Quantidade total de aviões gerados: 37
Quantidade de emergências finalizadas: 6 (16.2162%)
Quantidade de aviões *contabilizados* que caíram: 3 (8.10811%)
```

```
int seed = 789;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | -2 | 0

Avião HQ004 esperando para pousar com combustível atual -3 e prioridade 4
Avião AQ290 esperando para pousar com combustível atual -3 e prioridade 4
Avião WF848 esperando para pousar com combustível atual -1 e prioridade 5
Avião QF524 esperando para pousar com combustível atual -3 e prioridade 3
Avião MP663 esperando para pousar com combustível atual 1 e prioridade 5
Avião JX576 esperando para pousar com combustível atual 5 e prioridade 5
Avião XA572 esperando para pousar com combustível atual 5 e prioridade 5
Avião LA353 esperando para decolar com tempo de voo atual 9 e prioridade 3
Avião IB337 esperando para decolar com tempo de voo atual 8 e prioridade 5
Avião EE553 esperando para decolar com tempo de voo atual 8 e prioridade 5
Avião NO298 esperando para decolar com tempo de voo atual 7 e prioridade 5
Avião XS018 esperando para decolar com tempo de voo atual 6 e prioridade 4
Avião SB465 esperando para decolar com tempo de voo atual 5 e prioridade 5
Avião GH250 esperando para decolar com tempo de voo atual 5 e prioridade 4
Avião BQ353 esperando para decolar com tempo de voo atual 5 e prioridade 3
Avião DV381 esperando para decolar com tempo de voo atual 4 e prioridade 4
Avião MW956 esperando para decolar com tempo de voo atual 2 e prioridade 5
Avião FQ397 esperando para decolar com tempo de voo atual 2 e prioridade 5
Avião YW227 esperando para decolar com tempo de voo atual 1 e prioridade 4
Avião VC401 esperando para decolar com tempo de voo atual 1 e prioridade 1
Avião AF094 esperando para decolar com tempo de voo atual 0 e prioridade 4

Tempo médio de espera (pousos): 1
Tempo médio de espera (decolagens): 2
Quantidade média de combustível (esperando pousar): 0.142857
Quantidade média de combustível (já pousaram): 3.75
Quantidade total de aviões gerados: 44
Quantidade de emergências finalizadas: 8 (18.1818%)
Quantidade de aviões *contabilizados* que caíram: 3 (6.81818%)
```

Com um `T` pequeno e `K` grande, observa-se:
- Em média, 0.9443 tempos para pouso e 1.5766 tempos para decolagem
- Média-alta taxa de emergências finalizadas (em média, 13.4862%)
- Média taxa de aviões caídos (em média, 8.10811%), com poucos aviões presos na fila que estão com combustível negativos, mas muitos com tempo de voo excedente.

### Teste 04
`T` grande, `K` grande, `Pp == Pd`, `Pe` pequeno, `C` médio, `V` médio.
```C++
T = 1000; K = 6; Pp = 0.5; Pe = 0.1; C = 5; V = 5;
```

```
int seed = 123;

// output
RELATÓRIO DA RODADA:
Pistas: -2 | -2 | 0

Avião XA572 esperando para pousar com combustível atual -985 e prioridade 5
Avião TA540 esperando para pousar com combustível atual -984 e prioridade 5
Avião NW024 esperando para pousar com combustível atual -984 e prioridade 5
Avião PT012 esperando para pousar com combustível atual -973 e prioridade 5
Avião IW582 esperando para pousar com combustível atual -969 e prioridade 5
Avião ZR221 esperando para pousar com combustível atual -968 e prioridade 5
Avião RV465 esperando para pousar com combustível atual -959 e prioridade 5
Avião KX292 esperando para pousar com combustível atual -952 e prioridade 5
Avião JZ985 esperando para pousar com combustível atual -943 e prioridade 5
Avião NU815 esperando para pousar com combustível atual -928 e prioridade 5
...
Avião XW481 esperando para decolar com tempo de voo atual 6 e prioridade 5
Avião NI320 esperando para decolar com tempo de voo atual 5 e prioridade 3
Avião BO904 esperando para decolar com tempo de voo atual 4 e prioridade 5
Avião YZ785 esperando para decolar com tempo de voo atual 3 e prioridade 3
Avião WO881 esperando para decolar com tempo de voo atual 3 e prioridade 4
Avião MU201 esperando para decolar com tempo de voo atual 2 e prioridade 2
Avião PF054 esperando para decolar com tempo de voo atual 1 e prioridade 4
Avião QH430 esperando para decolar com tempo de voo atual 0 e prioridade 2
Avião QC616 esperando para decolar com tempo de voo atual 0 e prioridade 5
Avião BP048 esperando para decolar com tempo de voo atual 0 e prioridade 2

Tempo médio de espera (pousos): 2.95254
Tempo médio de espera (decolagens): 41.3484
Quantidade média de combustível (esperando pousar): -362.58
Quantidade média de combustível (já pousaram): 0.613559
Quantidade total de aviões gerados: 3557
Quantidade de emergências finalizadas: 1113 (31.2904%)
Quantidade de aviões *contabilizados* que caíram: 705 (19.8201%)
```

Com um `T` grande e `K` grande, é catastrófico. Foram tantas linhas de output de aviões enclausurados na fila que senti a necessidade de ocultar do leitor. Não é necessário nem mostrar os outros testes com outras seeds, visto que, em média, os resultados serão ruins.

# Conclusão

Infelizmente, esse algoritmo não é o mais eficiente nem o mais elegante, mas a experiência de ter um primeiro contato com C++ e tentar construir um sistema foi enriquecedora.
Aguardo os feedbacks.
