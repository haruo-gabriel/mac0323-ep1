// Como compilar e executar 
// clear && g++ -Wall -std=c++11 *.cpp -o a.out && ./a.out

#include "aerolib.hpp"

// se comporta como um minHeap
auto comparaAviao = [](Aviao* a1, Aviao* a2) {
    return a1->prioridade > a2->prioridade;
};

int main() {

    int T, K, C, V;
    float Pp, Pe; // Pd = 1 - Pp
    int numEmergenciasGeradasDesviadas=0, numEmergenciasOutrasDesviadas=0, totalAvioes=0;
    float tempoMedioEsperaPouso=0, tempoMedioEsperaDecolagem=0, qtdeMediaCombustivelEsperandoPousar=0, qtdeMediaCombustivelPousaram=0;

    int seed = 123;

    // modo interativo
    // cout << "Digite T, K, Pp, Pe, C e V, respectivamente: ";
    // cin >> T >> K >> Pp >> Pe >> C >> V; 

    // modo automático
    T = 1000; K = 2; Pp = 0.5; Pe = 1.0; C = 2; V = 2;

    srand(seed);

    vector<Pista> pistas; gerarPistas(pistas);
    list<Aviao*> pousosForaQueue;
    list<Aviao*> decolagensForaQueue;
    list<Aviao*> pousosNaQueue;
    list<Aviao*> decolagensNaQueue;
    list<Aviao*> avioesMortos;
    priority_queue<Aviao*, vector<Aviao*>, decltype(comparaAviao)> queue(comparaAviao);

    for (int t = 1; t <= T; t++) {
        // relatório pré-rodada
        cout << "TEMPO " << t << endl;
        if (t > 1) {
            cout << endl;
            cout << "RELATÓRIO PRÉ-RODADA:" << endl;
            cout << "    Pistas: " << pistas[0].livre << " | " << pistas[1].livre << " | " << pistas[2].livre << endl << endl;
            for (Aviao* it : pousosNaQueue) {
                cout << "    Avião " << it->id << " esperando para pousar com combustível " << (it->combustivel - it->contador) << " restante e prioridade " << it->prioridade << endl;
            }
            for (Aviao* it : decolagensNaQueue) {
                cout << "    Avião " << it->id << " esperando para decolar com tempo de voo atual " << it->contador << " e prioridade " << it->prioridade << endl;
            }
        }
        cout << endl;

        // gerar aviões
        int nAvioes = probAvioesNaRodada(K);
        cout << "Chegaram [" << nAvioes << "] aviões:" << endl;
        for (int i = 0; i < nAvioes; i++) {
            bool ehPouso = setEhPouso(Pp);
            Aviao* aviao;
            if (ehPouso) {
                aviao = new Aviao(Pp, Pe, ehPouso, C, -1, t);
                pousosNaQueue.push_back(aviao);
                cout << "    Avião " << aviao->id << " esperando para pousar com prioridade " << aviao->prioridade << endl;
            } else {
                aviao = new Aviao(Pp, Pe, ehPouso, -1, V, t);
                decolagensNaQueue.push_back(aviao);
                cout << "    Avião " << aviao->id << " esperando para decolar com prioridade " << aviao->prioridade << endl;
            }
            queue.push(aviao);
            totalAvioes++;
        } 
        cout << endl << "----------------------------------------" << endl << endl;

        // botar aviões nas pistas
        cout << "Selecionando aviões para pistas:" << endl;
        int i = 0;
        while (!queue.empty()
        && (pistas[0].livre == 1 || pistas[1].livre == 1 || (pistas[2].livre == 1 && !queue.top()->ehPouso))) {
            cout << "   Avião " << queue.top()->id;
            // aviões com combustível negativo não entram em pistas
            if (queue.top()->ehPouso && queue.top()->combustivel < queue.top()->contador) {
                cout << " caiu por falta de combustivel" << endl;
                pousosNaQueue.remove(queue.top());
                avioesMortos.push_back(queue.top());
            } else {
                for (; i < 3; i++) {
                    if (pistas[i].livre == 1 && !(i == 2 && queue.top()->ehPouso)) {
                        cout << " com prioridade " << queue.top()->prioridade << " alocado na pista " << i << endl;
                        if (queue.top()->ehPouso) {
                            tempoMedioEsperaPouso += queue.top()->contador;
                            qtdeMediaCombustivelPousaram += (queue.top()->combustivel - queue.top()->contador);
                            pousosNaQueue.remove(queue.top());
                            pousosForaQueue.push_back(queue.top());
                        } else {
                            tempoMedioEsperaDecolagem += queue.top()->contador;
                            decolagensNaQueue.remove(queue.top());
                            decolagensForaQueue.push_back(queue.top());
                        }
                        pistas[i].cooldown();
                        break;
                    }
                }
            }
            queue.pop();
        }
        cout << endl;

        // aqui não há mais pistas livres, mas deve-se lidar com as
        // emergências e desviá-las para outro aeroporto
        // ou contabilizar quais aviões caíram
        cout << "Lidando com as emergências:" << endl;
        while (!queue.empty() && (queue.top()->prioridade == 0
        || (queue.top()->ehPouso && (queue.top()->combustivel <= queue.top()->contador))
        || (!queue.top()->ehPouso && (queue.top()->contador > 1.1*queue.top()->tempoVoo)))) {
            cout << "    Avião " << queue.top()->id;
            if (queue.top()->ehPouso) {
                // avioes que caíram
                if (queue.top()->combustivel < queue.top()->contador) {
                    cout << " caiu por falta de combustivel" << endl;
                    avioesMortos.push_back(queue.top());
                } else {
                    if (queue.top()->prioridade == 0) {
                        cout << " foi desviado por emergência" << endl;
                        qtdeMediaCombustivelPousaram += (queue.top()->combustivel - queue.top()->contador);
                        numEmergenciasGeradasDesviadas++;
                    } else if (queue.top()->combustivel == queue.top()->contador) {
                        cout  << " foi desviado por falta de combustível" << endl;
                        qtdeMediaCombustivelPousaram += 0;
                        numEmergenciasOutrasDesviadas++;
                    }
                    tempoMedioEsperaPouso += queue.top()->contador;
                }
                pousosNaQueue.remove(queue.top());
            } else {
                if (queue.top()->prioridade == 0) {
                    cout << " foi desviado por emergência" << endl;
                    numEmergenciasGeradasDesviadas++;
                } else if (queue.top()->contador > 1.1*queue.top()->tempoVoo){
                    cout <<  " foi desviado por exceder o limite de tempo de voo" << endl;
                    numEmergenciasOutrasDesviadas++;
                }
                tempoMedioEsperaDecolagem += queue.top()->contador;
                decolagensNaQueue.remove(queue.top());
                decolagensForaQueue.push_back(queue.top());
            }
            queue.pop();
        }
        cout << endl << "----------------------------------------" << endl << endl;

        // relatório da rodada e estatísticas
        cout << "RELATÓRIO DA RODADA:" << endl;
        cout << "    Pistas: " << pistas[0].livre << " | " << pistas[1].livre << " | " << pistas[2].livre << endl;
        cout << endl;

        qtdeMediaCombustivelEsperandoPousar=0;
        for (Aviao* it : pousosNaQueue) {
            cout << "    Avião " << it->id << " esperando para pousar com combustível atual " << (it->combustivel - it->contador) << " e prioridade " << it->prioridade << endl;
            qtdeMediaCombustivelEsperandoPousar += (it->combustivel - it->contador); 
        }
        for (Aviao* it : decolagensNaQueue) {
            cout << "    Avião " << it->id << " esperando para decolar com tempo de voo atual " << it->contador << " e prioridade " << it->prioridade << endl;
        }

        cout << endl;
        cout << "    Tempo médio de espera (pousos): " << tempoMedioEsperaPouso/pousosForaQueue.size() << endl;
        cout << "    Tempo médio de espera (decolagens): " << tempoMedioEsperaDecolagem/decolagensForaQueue.size() << endl;
        cout << "    Quantidade média de combustível (esperando pousar): " << qtdeMediaCombustivelEsperandoPousar/pousosNaQueue.size() << endl;
        cout << "    Quantidade média de combustível (já pousaram): " << qtdeMediaCombustivelPousaram/pousosForaQueue.size() << endl;
        cout << "    Quantidade total de aviões gerados: " << totalAvioes << endl;
        cout << "    Quantidade de emergências (gerado como emergência) desviadas: " << numEmergenciasGeradasDesviadas << " (" << (float)numEmergenciasGeradasDesviadas/totalAvioes*100 << "%)" << endl;
        cout << "    Quantidade de emergências (outras) desviadas: " << numEmergenciasOutrasDesviadas << " (" << (float)numEmergenciasOutrasDesviadas/totalAvioes*100 << "%)" << endl;
        cout << "    Quantidade de aviões que caíram: " << avioesMortos.size() << " (" << (float)avioesMortos.size()/totalAvioes*100 << "%)" << endl;
        cout << endl << "================================================================================" << endl << endl;

        // atualizar contadores e cooldown
        for (int i = 0; i < 3; i++)
            if (pistas[i].livre < 1) pistas[i].livre++;
        for (Aviao* it : pousosNaQueue)
            it->contador++;
        for (Aviao* it : decolagensNaQueue)
            it->contador++;
    }

    // liberar memória
    pistas.clear();
    pousosNaQueue.clear();
    decolagensNaQueue.clear();
    pousosForaQueue.clear();
    decolagensForaQueue.clear();
    avioesMortos.clear();
    while (!queue.empty())
        queue.pop();

    return 0;
}