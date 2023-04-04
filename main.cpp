// EP1 de MAC0323
// Nome: Gabriel Haruo Hanai Takeuchi
// NUSP: 13671636

#include "aerolib.hpp"
#include <iostream>

int main() {
    srand(SRAND);

    int T, K, C, V, haPistaLivre;
    float Pp, Pe; // Pd = 1 - Pp
    Aviao* aviao;

    vector<Pista> pistas;
    gerarPistas(pistas);

    vector<Aviao*> avioesPouso;
    vector<Aviao*> avioesDecolagem;
    priority_queue<Aviao*, std::vector<Aviao*>, decltype(comparaAviao)> avioesQueue(comparaAviao);

    cout << "Digite T, K, Pp, Pe, C e V, respectivamente: ";
    cin >> T >> K >> Pp >> Pe >> C >> V; 

    // TODO pensar nesse loop no papel!
    for (int t = 0; t < T; t++) {
        // gerar aviões
        int nAvioes = probAvioes(K);

        for (int i = 0; i < nAvioes; i++) {
            bool ehPouso = setEhPouso(Pp);
            if (ehPouso) {
                aviao = new AviaoPouso(Pp, Pe, ehPouso, C);
                avioesPouso.push_back(aviao);
                avioesQueue.push(avioesPouso.back());
                avioesPouso.back()->naQueue = true;
            } else {
                aviao = new AviaoDecolagem(Pp, Pe, ehPouso, V);
                avioesQueue.push(avioesDecolagem.back());
                avioesDecolagem.back()->naQueue = true;
            }
        } 

        // botar aviões nas pistas
        if (avioesQueue.top()->ehPouso) {
            for (int i = 0; i < 3; i++) {
                if (pistas[i].livre){
                    if (pistas[i].podePousar){
                        avioesQueue.top()->naQueue = false;
                        avioesQueue.pop();
                        pistaCooldown(pistas, i);
                    }
                }

            }
        } else {

        }

        // atualizar valores de combustível e cooldown
    }

    return 0;
}