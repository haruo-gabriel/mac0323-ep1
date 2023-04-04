#include "aerolib.hpp"

Pista::Pista(bool podeDecolar, bool podePousar) {
    this->podeDecolar = podeDecolar;
    this->podePousar = podePousar;
    this->livre = 1;
}

// Constructors
Aviao::Aviao(float Pp, float Pe, bool ehPouso) {
    this->id = setId();
    this->contador = 0;
    this->ehPouso = ehPouso;
    this->prioridade = setPrioridade(Pe);
    this->codigo = setCodigo(this->ehPouso);
};

AviaoPouso::AviaoPouso(float Pp, float Pe, bool ehPouso, int C) : Aviao(Pp, Pe, ehPouso) {
    this->combustivel = setCombustivel(C);
};

AviaoDecolagem::AviaoDecolagem(float Pp, float Pe, bool ehPouso, int V) : Aviao(Pp, Pe, ehPouso) {
    this->tempoVoo = setTempoVoo(V);
};


// Métodos
string Aviao::setId() {
    string temp(1, 'A' + (rand() % 26));
    temp += 'A' + (rand() % 26);
    for (int i = 0; i < 3; i++)
        temp += '0' + (rand() % 10);
}


int Aviao::setPrioridade(float Pe) {
    if ( (float)rand() / RAND_MAX < Pe )
        return 3;
}

string Aviao::setCodigo(bool ehPouso) {
    string temp;
    temp += ehPouso ? 'P' : 'D';
    temp += 'A' + (rand() % 26);
    temp += 'A' + (rand() % 26);
    return temp;
}

int AviaoPouso::setCombustivel(int C) {
    return rand() % C + 1;
}

int AviaoDecolagem::setTempoVoo(int V) {
    return rand() % V + 1;
}


void gerarPistas(vector<Pista> &pistas) {
    pistas.push_back(Pista(1, 1)); // pistas[0] é a pista 1
    pistas.push_back(Pista(1, 1)); // pistas[1] é a pista 2
    pistas.push_back(Pista(1, 0)); // pistas[2] é a pista 3, a exclusiva de decolagem
}

bool setEhPouso(float Pp) {
    return (float)rand() / RAND_MAX < Pp;
}

auto comparaAviao = [](Aviao* a1, Aviao* a2) {
    return a1->prioridade > a2->prioridade;
};

int probAvioes(int K) {
    return rand() % (K + 1);
}