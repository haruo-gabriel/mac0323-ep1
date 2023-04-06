#ifndef AEROLIB_HPP
#define AEROLIB_HPP

#include <string>
#include <vector>
#include <queue>
#include <cstdlib> 
#include <iostream>

#define SRAND 1111

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
    bool ehPouso, naQueue;

    Aviao(float Pp, float Pe, bool ehPouso, int C, int V);

    string setId();
    int setPrioridade(float Pe);
    string setCodigo(bool ehPouso);
    int setCombustivel(int C);
    int setTempoVoo(int V);
};

// class AviaoPouso: public Aviao {
// public:
//     int combustivel;

//     AviaoPouso(float Pp, float Pe, bool ehPouso, int C);

//     int setCombustivel(int C);
// };

// class AviaoDecolagem: public Aviao {
// public:
//     int tempoVoo;

//     AviaoDecolagem(float Pp, float Pe, bool ehPouso, int V);
    
//     int setTempoVoo(int V);
// };


// Funções
void gerarPistas(vector<Pista> &pistas);

bool haPistaLivre(vector<Pista> &pistas);

bool setEhPouso(float Pp);

int probAvioes(int K);

#endif