#ifndef AEROLIB_HPP
#define AEROLIB_HPP

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <cstdlib> 
#include <iostream>

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
    bool ehPouso;

    Aviao(float Pp, float Pe, bool ehPouso, int C, int V, int t);

    string setId();
    string setCodigo(bool ehPouso);
    int setCombustivel(int C);
    int setTempoVoo(int V);
    int setPrioridade(float Pe, int combustivel, int tempoVoo, int t);
};


// Funções
void gerarPistas(vector<Pista> &pistas);
bool setEhPouso(float Pp);
int probAvioesNaRodada(int K);

#endif