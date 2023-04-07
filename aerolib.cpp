#include "aerolib.hpp"

// Construtores
Pista::Pista() {
    this->livre = 1;
}

Aviao::Aviao(float Pp, float Pe, bool ehPouso, int C, int V) {
    this->contador = 0;
    this->ehPouso = ehPouso;
    this->id = setId();
    this->codigo = setCodigo(ehPouso);
    this->combustivel = setCombustivel(C);
    this->tempoVoo = setTempoVoo(V);
    this->prioridade = setPrioridade(Pe, this->combustivel, this->tempoVoo);
};


// Métodos
void Pista::cooldown() {
    this->livre = -2;
}


string Aviao::setId() {
    string temp(1, 'A' + (rand() % 26));
    temp += 'A' + (rand() % 26);
    for (int i = 0; i < 3; i++)
        temp += '0' + (rand() % 10);
    return temp;
}

string Aviao::setCodigo(bool ehPouso) {
    string temp;
    temp += 'A' + (rand() % 26);
    temp += ehPouso ? 'P' : 'D';
    temp += 'A' + (rand() % 26);
    return temp;
}

int Aviao::setCombustivel(int C) {
    return C < 0 ? -1 : rand() % C + 1;
}

int Aviao::setTempoVoo(int V) {
    return V < 0 ? -1 : rand() % V + 1;
}

int Aviao::setPrioridade(float Pe, int C, int V) {
    if ((float)rand() / RAND_MAX < Pe)
        return 0;
    else if (C == -1)
        return V;
    // else if (V == -1)
    return C;
}


// Funções
void gerarPistas(vector<Pista> &pistas) {
    pistas.push_back(Pista());
    pistas.push_back(Pista());
    pistas.push_back(Pista()); // exclusiva para decolagens
}

bool setEhPouso(float Pp) {
    return (float)rand() / RAND_MAX < Pp;
}

int probAvioesNaRodada(int K) {
    return rand() % K + 1;
}