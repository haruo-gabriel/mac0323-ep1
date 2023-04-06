// Como compilar
// g++ -g -Wall -std=c++11 *.cpp -o a.out

#include "aerolib.hpp"

auto comparaAviao = [](Aviao* a1, Aviao* a2) {
    cout << "comparando " << a1->id << " e " << a2->id << endl;
    cout << "a1->prioridade " << a1->prioridade << endl;
    cout << "a2->prioridade " << a2->prioridade << endl;
    cout << endl;
    return a1->prioridade < a2->prioridade;
};

int main() {
    srand(SRAND);

    int T, K, C, V;
    float Pp, Pe; // Pd = 1 - Pp
    int numEmergenciasTerminadas=0, numPousos=0, numDecolagens=0;
    float tempoMedioEsperaPouso=0, tempoMedioEsperaDecolagem=0, qtdeMediaCombustivelEsperandoPousar=0, qtdeMediaCombustivelPousaram=0;
    // TODO dar um jeito de colocar qtdeMediaCombustivelEsperandoPousar.
    // TODO criar uma lista ligada só com os que não estão na fila, para poder calcular a média de combustível deles.

    vector<Pista> pistas; gerarPistas(pistas);
    vector<Aviao*> avioesVector;
    priority_queue<Aviao*, vector<Aviao*>, decltype(comparaAviao)> avioesQueue(comparaAviao);

    // cout << "Digite T, K, Pp, Pe, C e V, respectivamente: ";
    // cin >> T >> K >> Pp >> Pe >> C >> V; 
    T = MAXRODADAS; K = 3; Pp = 0.5; Pe = 0.05; C = 5; V = 5;

    for (int t = 0; t < T; t++) {
        // relatório pré-rodada
        cout << "Tempo " << t+1 << endl;
        cout << endl;
        if (t > 0) {
            cout << "Relatório pré-rodada:" << endl;
            cout << "Pistas: " << pistas[0].livre << " | " << pistas[1].livre << " | " << pistas[2].livre << endl << endl;
            for (Aviao* it : avioesVector) {
                if (it->naQueue) {
                    if (it->ehPouso) {
                        cout << "Avião " << it->id << " esperando para pousar com combustível " << (it->combustivel - it->contador) << " e prioridade " << it->prioridade << endl;
                    } else {
                        cout << "Avião " << it->id << " esperando para decolar com tempo de voo " << (it->tempoVoo - it->contador) << " e prioridade " << it->prioridade << endl;
                    }
                }
                cout << endl;
            }

        // gerar aviões
        int nAvioes = probAvioes(K);
        cout << "Chegaram " << nAvioes << " aviões" << endl << endl;
        for (int i = 0; i < nAvioes; i++) {
            bool ehPouso = setEhPouso(Pp);
            Aviao* aviao;
            if (ehPouso) {
                aviao = new Aviao(Pp, Pe, ehPouso, C, -1);
                numPousos++:
            } else {
                aviao = new Aviao(Pp, Pe, ehPouso, -1, V);
                numDecolagens++;
            }
            avioesVector.push_back(aviao);
            avioesQueue.push(aviao);
            avioesVector.back()->naQueue = true;
        } 

        // botar aviões nas pistas
        while (!avioesQueue.empty() && avioesQueue.top()->naQueue && (pistas[0].livre == 1 || pistas[1].livre == 1 || (pistas[2].livre == 1 && !avioesQueue.top()->ehPouso))) {
            cout << "Avião de maior prioridade: " << endl;
            cout << "id: " << avioesQueue.top()->id << endl;
            cout << "prioridade: " << avioesQueue.top()->prioridade << endl;
            cout << "ehPouso: " << avioesQueue.top()->ehPouso << endl;
            if (avioesQueue.top()->ehPouso) cout << "combustivel: " << (avioesQueue.top()->combustivel - avioesQueue.top()->contador) << endl;
            else cout << "tempoVoo: " << (avioesQueue.top()->tempoVoo - avioesQueue.top()->contador) << endl;
            cout << endl;

            for (int i = 0; i < 3; i++) {
                if (pistas[i].livre == 1) {
                    if (i == 0 || i == 1 || (i == 2 && !avioesQueue.top()->ehPouso)) {
                        if (avioesQueue.top()->ehPouso) {
                            tempoMedioEsperaPouso += avioesQueue.top()->contador;
                            qtdeMediaCombustivelPousaram += avioesQueue.top()->combustivel - avioesQueue.top()->contador;
                        } else {
                            tempoMedioEsperaDecolagem += avioesQueue.top()->contador;
                        }
                        avioesQueue.top()->naQueue = false;
                        avioesQueue.pop();
                        pistas[i].cooldown();
                    }
                    break;
                }
            }
        }

        // nesse ponto, não há pistas livre, mas deve-se lidar com as
        // emergências e desviá-las para outro aeroporto
        while (!avioesQueue.empty() && avioesQueue.top()->naQueue) {
            cout << "Emergências!" << endl;
            cout << "Avião de maior prioridade: " << endl;
            cout << "id: " << avioesQueue.top()->id << endl;
            cout << "prioridade: " << avioesQueue.top()->prioridade << endl;
            cout << "ehPouso: " << avioesQueue.top()->ehPouso << endl;

            if (avioesQueue.top()->ehPouso) {
                cout << "combustivel: " << (avioesQueue.top()->combustivel - avioesQueue.top()->contador) << endl;
                tempoMedioEsperaPouso += avioesQueue.top()->contador;
            }
            else{
                cout << "tempoVoo: " << (avioesQueue.top()->tempoVoo - avioesQueue.top()->contador) << endl;
                tempoMedioEsperaDecolagem += avioesQueue.top()->contador;
            }
            cout << endl;
            avioesQueue.top()->naQueue = false;
            avioesQueue.pop();
            numEmergenciasTerminadas++;
        }

        // relatório da rodada
        cout << "Relatório da rodada:" << endl;
        cout << "Pistas: " << pistas[0].livre << " | " << pistas[1].livre << " | " << pistas[2].livre << endl;
        cout << endl;
        for (Aviao* it : avioesVector) {
            if (it->naQueue) {
                if (it->ehPouso) {
                    cout << "Avião " << it->id << " esperando para pousar com combustível " << (it->combustivel - it->contador) << " e prioridade " << it->prioridade << endl;
                    qtdeMediaCombustivelEsperandoPousar += (it->combustivel - it->contador); 
                } else {
                    if (it->ehPouso) {
                        cout << "Avião " << it->id << " esperando para decolar com tempo de voo " << (it->tempoVoo - it->contador) << " e prioridade " << it->prioridade << endl;
                    }
                }
            } 
        }
        cout << endl;
        cout << "Tempo médio de espera para pousar: " << tempoMedioEsperaPouso/numPousos << endl;
        cout << "Tempo médio de espera para decolar: " << tempoMedioEsperaDecolagem/numDecolagens << endl;
        cout << "Quantidade média de combustível dos que ainda não pousaram: " << qtdeMediaCombustivelEsperandoPousar/avioesVector.size() << endl;
        cout << "Quantidade média de combustível dos que já pousaram: " << qtdeMediaCombustivelPousaram/avioesVector.size() << endl;
        cout << "Quantidade de emergências finalizadas: " << numEmergenciasTerminadas << endl;
        cout << endl << "----------------------------------------" << endl << endl;

        // atualizar valores de combustível e cooldown
        for (int i = 0; i < 3; i++) {
            if (pistas[i].livre < 1) pistas[i].livre++;
        }
        for (Aviao* it : avioesVector) {
            if (it->naQueue)
                it->contador++;
        }
    }

    return 0;
}