// EP1 de MAC0323
// Nome: Gabriel Haruo Hanai Takeuchi
// NUSP: 13671636

#include "aerolib.hpp"
#include <iostream>

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