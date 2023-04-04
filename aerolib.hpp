#ifndef AEROLIB_HPP
#define AEROLIB_HPP

#include <string>
#include <vector>
#include <queue>

#define SRAND 100

using namespace std;

// Classes
class Pista {
public:
    bool podeDecolar, podePousar;
    int livre;
    // A cada rodada, livre é incrementado.
    // A pista está livre se livre >= 1, e ocupada se livre <= 0.
    // Se for ocupada, imediatamente muda seu valor para -2 (cooldown).

    Pista(bool podeDecolar, bool podePousar);
};

class Aviao {
public:
    string id, codigo;
    int prioridade, contador;
    bool ehPouso, naQueue;

    Aviao(float Pp, float Pe, bool ehPouso);

    string setId();
    int setPrioridade(float Pe);
    string setCodigo(bool ehPouso);
};

class AviaoPouso: public Aviao {
public:
    int combustivel;

    AviaoPouso(float Pp, float Pe, bool ehPouso, int C);

    int setCombustivel(int C);
};

class AviaoDecolagem: public Aviao {
public:
    int tempoVoo;

    AviaoDecolagem(float Pp, float Pe, bool ehPouso, int V);
    
    int setTempoVoo(int V);
};


// Funções
void gerarPistas(vector<Pista> &pistas);

bool setEhPouso(float Pp);

auto comparaAviao = [](Aviao* a1, Aviao* a2) {
    return a1->prioridade > a2->prioridade;
};

int probAvioes(int K);

#endif