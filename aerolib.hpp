#ifndef AEROLIB_HPP
#define AEROLIB_HPP

#include <string>
#include <vector>
#include <queue>
#include <cstdlib> 
#include <iostream>
#include <functional>

#define SRAND 1111
#define MAXRODADAS 99

using namespace std;

// Classes
class Pista {
public:
    int livre;

    Pista();

    void cooldown();
};

class Aviao {
public:
    string id, codigo;
    int prioridade, contador, combustivel, tempoVoo;
    // se pouso, tempoVoo = -1
    // se decolagem, combustivel = -1
    bool ehPouso, naQueue;

    Aviao(float Pp, float Pe, bool ehPouso, int C, int V);

    string setId();
    string setCodigo(bool ehPouso);
    int setCombustivel(int C);
    int setTempoVoo(int V);
    int setPrioridade(float Pe, int C, int V);
};


// Funções
void gerarPistas(vector<Pista> &pistas);

bool haPistaLivre(vector<Pista> &pistas);

bool setEhPouso(float Pp);

int probAvioes(int K);

using AviaoComparador = function<bool(Aviao*, Aviao*)>;
extern AviaoComparador comparaAviao;
void reconstroiQueue(priority_queue<Aviao*, vector<Aviao*>, decltype(comparaAviao)> queue, Aviao* aviaoRemover);

#endif