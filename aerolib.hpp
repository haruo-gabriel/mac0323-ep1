#ifndef AEROLIB_HPP
#define AEROLIB_HPP

#include <string>
#include <vector>
#include <queue>

#define SRAND 100

using namespace std;

class Pista {
public:
    bool podeDecolar;
    bool podePousar;
    int livre;
    // A cada rodada, livre é incrementado.
    // A pista está livre se livre >= 1, e ocupada se livre <= 0.
    // Se for ocupada, imediatamente muda seu valor para -2 (cooldown).

    Pista(bool podeDecolar, bool podePousar);
};

class Aviao {
public:
    string id, codigo;
    int combustivel, tempoVoo;
    bool ehEmergencia, ehPouso;

    Aviao(float Pp, float Pe, int C, int V);

    string setId();
    bool setEhPouso(float Pp);
    bool setEhEmergencia(float Pe);
    int setCombustivel(int C);
    int setTempoVoo(int V);
    string setCodigo(bool ehPouso);
};

void gerarPistas(vector<Pista> &pistas);
auto comparaAviao = [](Aviao* a1, Aviao* a2) {
    return a1->ehEmergencia > a2->ehEmergencia;
};
int probAvioes(int K);

#endif