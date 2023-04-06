#include "aerolib.hpp"


// Constructors
Pista::Pista() {
    this->livre = 1;
}

Aviao::Aviao(float Pp, float Pe, bool ehPouso, int C, int V) {
    this->id = setId();
    this->contador = 0;
    this->ehPouso = ehPouso;
    this->prioridade = setPrioridade(Pe);
    this->codigo = setCodigo(ehPouso);
    this->naQueue = false;
    this->combustivel = setCombustivel(C);
    this->tempoVoo = setTempoVoo(V);
    // cout << "Aviao " << this->id << " criado" << endl;
};

// AviaoPouso::AviaoPouso(float Pp, float Pe, bool ehPouso, int C) : Aviao(Pp, Pe, ehPouso) {
//     this->combustivel = setCombustivel(C);
//     // cout << "Aviao POUSO " << this->id << " criado" << endl;
// };

// AviaoDecolagem::AviaoDecolagem(float Pp, float Pe, bool ehPouso, int V) : Aviao(Pp, Pe, ehPouso) {
//     this->tempoVoo = setTempoVoo(V);
//     // cout << "Aviao DECOLAGEM " << this->id << " criado" << endl;
// };


// Métodos
void Pista::cooldown() {
    this->livre = -1;
}

string Aviao::setId() {
    string temp(1, 'A' + (rand() % 26));
    temp += 'A' + (rand() % 26);
    for (int i = 0; i < 3; i++)
        temp += '0' + (rand() % 10);
    return temp;
}


int Aviao::setPrioridade(float Pe) {
    return ((float)rand() / RAND_MAX < Pe) ? 2 : 0;
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

// int AviaoPouso::setCombustivel(int C) {
//     return rand() % C + 1;
// }

// int AviaoDecolagem::setTempoVoo(int V) {
//     return rand() % V + 1;
// }


void gerarPistas(vector<Pista> &pistas) {
    pistas.push_back(Pista()); // pistas[0] é a pista 1
    pistas.push_back(Pista()); // pistas[1] é a pista 2
    pistas.push_back(Pista()); // pistas[2] é a pista 3, a exclusiva de decolagem
}

bool haPistaLivre(vector<Pista> &pistas) {
    for (int i = 0; i < 3; i++)
        if (pistas[i].livre == 1)
            return true;
    return false;
}

bool setEhPouso(float Pp) {
    return (float)rand() / RAND_MAX < Pp;
}


int probAvioes(int K) {
    return rand() % K + 1;
}