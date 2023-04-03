#include "aerolib.hpp"

Pista::Pista(bool podeDecolar, bool podePousar) {
    this->podeDecolar = podeDecolar;
    this->podePousar = podePousar;
    this->livre = true;
}


Aviao::Aviao(float Pp, float Pe, int C, int V) {
    this->id = setId();
    this->ehPouso = setEhPouso(Pp);
    this->ehEmergencia = setEhEmergencia(Pe);
    this->combustivel = setCombustivel(C);
    this->tempoVoo = setTempoVoo(V);
    this->codigo = setCodigo(this->ehPouso);
}

string Aviao::setId() {
    string temp(1, 'A' + (rand() % 26));
    temp += 'A' + (rand() % 26);
    for (int i = 0; i < 3; i++)
        temp += '0' + (rand() % 10);
}

bool Aviao::setEhPouso(float Pp) {
    return (float)rand() / RAND_MAX < Pp;
}

bool Aviao::setEhEmergencia(float Pe) {
    return (float)rand() / RAND_MAX < Pe;
}

int Aviao::setCombustivel(int C) {
    return rand() % C + 1;
}

int Aviao::setTempoVoo(int V) {
    return rand() % V + 1;
}

string Aviao::setCodigo(bool ehPouso) {
    string temp;
    temp += ehPouso ? 'P' : 'D';
    temp += 'A' + (rand() % 26);
    temp += 'A' + (rand() % 26);
    return temp;
}


void gerarPistas(vector<Pista> &pistas) {
    pistas.push_back(Pista(1, 1)); // pistas[0] é a pista 1
    pistas.push_back(Pista(1, 1)); // pistas[1] é a pista 2
    pistas.push_back(Pista(1, 0)); // pistas[2] é a pista 3, a exclusiva de decolagem
}

auto comparaAviao = [](Aviao* a1, Aviao* a2) {
    return a1->ehEmergencia > a2->ehEmergencia;
};

int probAvioes(int K) {
    return rand() % (K + 1);
}