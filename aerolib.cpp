#include "aerolib.hpp"


// Constructors
Pista::Pista() {
    this->livre = 1;
}

Aviao::Aviao(float Pp, float Pe, bool ehPouso, int C, int V) {
    this->id = setId();
    this->contador = 0;
    this->ehPouso = ehPouso;
    this->codigo = setCodigo(ehPouso);
    this->naQueue = false;
    this->combustivel = setCombustivel(C);
    this->tempoVoo = setTempoVoo(V);
    this->prioridade = setPrioridade(Pe, C, V);
    // cout << "Aviao " << this->id << " criado" << endl;
};


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
    if ((float)rand() / RAND_MAX < Pe) {
        return 0;
    } else if (C < 0) {
        return V;
    } else if (V < 0) {
        return C;
    }
}


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

// AviaoComparador comparaAviao = [](Aviao* a1, Aviao* a2) {
//     cout << "comparando " << a1->id << " e " << a2->id << endl;
//     cout << "a1->prioridade " << a1->prioridade << endl;
//     cout << "a2->prioridade " << a2->prioridade << endl;
//     cout << endl;
//     return a1->prioridade > a2->prioridade;
// };

// void reconstroiQueue(priority_queue<Aviao*, vector<Aviao*>, decltype(comparaAviao)> queue, Aviao* aviaoRemover) {
//     // Create a temporary container
//     vector<Aviao*> temp;

//     // Iterate over the elements in the priority_queue
//     while (!queue.empty()) {
//         Aviao* aviao= queue.top();
//         queue.pop();

//         // Check if the airplane matches the one to be removed
//         if (aviao == aviaoRemover) {
//             // Skip this airplane and don't add it to the temporary container
//         } else {
//             // Add the airplane to the temporary container
//             temp.push_back(aviao);
//         }
//     }

//     // Rebuild the priority_queue from the remaining elements in the temporary container
//     queue = priority_queue<Aviao*, vector<Aviao*>, decltype(comparaAviao)> queue(temp.begin(), temp.end(), comparaAviao);
// }
