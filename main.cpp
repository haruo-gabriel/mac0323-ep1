// EP1 de MAC0323
// Nome: Gabriel Haruo Hanai Takeuchi
// NUSP: 13671636

#include <bits/stdc++.h>

using namespace std;

#define SRAND 100

class Pista {
public:
    bool podeDecolar;
    bool podePousar;
    int livre;
    // A cada rodada, livre é incrementado.
    // A pista está livre se livre >= 1, e ocupada se livre <= 0.
    // Se for ocupada, imediatamente muda seu valor para -2 (cooldown).

    // Construtor
    Pista(bool podeDecolar, bool podePousar) {
        this->podeDecolar = podeDecolar;
        this->podePousar = podePousar;
        this->livre = true;
    }

    // Metodos
};

class Aviao {
public:
    string id, codigo;
    int combustivel, tempoVoo;
    bool ehEmergencia, ehPouso;

    // Construtor
    Aviao(float Pp, float Pe, int C, int V) {
        this->id = setId();
        this->ehPouso = setEhPouso(Pp);
        this->ehEmergencia = setEhEmergencia(Pe);
        this->combustivel = setCombustivel(C);
        this->tempoVoo = setTempoVoo(V);
        this->codigo = setCodigo(this->ehPouso);
    }

    // Metodos
    string setId() {
        string temp(1, 'A' + (rand() % 26));
        temp += 'A' + (rand() % 26);
        for (int i = 0; i < 3; i++)
            temp += '0' + (rand() % 10);
    }

    bool setEhPouso(float Pp) {
        return (float)rand() / RAND_MAX < Pp;
    }

    bool setEhEmergencia(float Pe) {
        return (float)rand() / RAND_MAX < Pe;
    }

    int setCombustivel(int C) {
        return rand() % C + 1;
    }

    int setTempoVoo(int V) {
        return rand() % V + 1;
    }

    string setCodigo(bool ehPouso) {
        string temp;
        temp += ehPouso ? 'P' : 'D';
        temp += 'A' + (rand() % 26);
        temp += 'A' + (rand() % 26);
        return temp;
    }
};

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

int main() {
    srand(SRAND);

    int T, K, C, V;
    float Pp, Pe; // Pd = 1 - Pp

    vector<Pista> pistas;
    gerarPistas(pistas);

    vector<Aviao> avioesVector;
    priority_queue<Aviao*, std::vector<Aviao*>, decltype(comparaAviao)> avioesQueue(comparaAviao);

    cout << "Digite T, K, Pp, Pe, C e V, respectivamente: ";
    cin >> T >> K >> Pp >> Pe >> C >> V; 

    // TODO pensar nesse loop no papel!
    for (int t = 0; t < T; t++) {
        int nAvioes = probAvioes(K);
        for (int i = 0; i < nAvioes; i++) {
            Aviao aviao(C, V, Pp, Pe);
            avioesVector.push_back(aviao);
            avioesQueue.push(&avioesVector.back());
        } 
    }

    return 0;
}