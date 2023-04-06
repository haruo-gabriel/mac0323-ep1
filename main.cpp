// Como compilar
// g++ -g -Wall -std=c++11 *.cpp -o a.out

#include "aerolib.hpp"

// template<typename T, typename Container = vector<T>, typename Compare = less<typename Container::value_type>>
// class custom_priority_queue : public priority_queue<T, Container, Compare> {
// public:
//     custom_priority_queue(const Compare& comp = Compare(), const Container& cont = Container())
//     : priority_queue<T, Container, Compare>(comp, cont) {}

//     bool remove(const T& value) {
//         auto it = find(this->c.begin(), this->c.end(), value);
//         if (it == this->c.end()) return false;
//         if (it == this->c.begin()) this->pop();
//         else {
//             this->c.erase(it);
//             make_heap(this->c.begin(), this->c.end(), this->comp);
//         }
//         return true;
//     }
// };

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
    int numEmergenciasTerminadas=0;
    float tempoMedioEsperaPouso=0, tempoMedioEsperaDecolagem=0, qtdeMediaCombustivelEsperandoPousar=0, qtdeMediaCombustivelPousaram=0;

    vector<Pista> pistas; gerarPistas(pistas);
    vector<Aviao*> avioesPouso;
    vector<Aviao*> avioesDecolagem;
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
            for (Aviao* it : avioesPouso) {
                if (it->naQueue)
                    cout << "Avião " << it->id << " esperando para pousar com combustível " << it->contador << " e prioridade " << it->prioridade << endl;
            }
            for (Aviao* it : avioesDecolagem) {
                if (it->naQueue)
                    cout << "Avião " << it->id << " esperando para decolar com tempo de voo " << it->contador << " e prioridade " << it->prioridade << endl;
            }
            cout << endl;
        }

        // gerar aviões
        int nAvioes = probAvioes(K);
        cout << "Chegaram " << nAvioes << " aviões" << endl << endl;
        for (int i = 0; i < nAvioes; i++) {
            bool ehPouso = setEhPouso(Pp);
            if (ehPouso) {
                Aviao* aviao = new Aviao(Pp, Pe, ehPouso, C, -1);
                // cout << "combustivel " << aviao->combustivel << endl;
                avioesPouso.push_back(aviao);
                avioesQueue.push(aviao);
                avioesPouso.back()->naQueue = true;
            } else {
                Aviao* aviao = new Aviao(Pp, Pe, ehPouso, -1, V);
                // cout << "tempoVoo " << aviao->tempoVoo << endl;
                avioesDecolagem.push_back(aviao);
                avioesQueue.push(aviao);
                avioesDecolagem.back()->naQueue = true;
            }
        } 

        // botar aviões nas pistas
        while (!avioesQueue.empty() && avioesQueue.top()->naQueue && (pistas[0].livre == 1 || pistas[1].livre == 1 || (pistas[2].livre == 1 && !avioesQueue.top()->ehPouso))) {
            cout << "Avião de maior prioridade: " << endl;
            cout << "id: " << avioesQueue.top()->id << endl;
            cout << "prioridade: " << avioesQueue.top()->prioridade << endl;
            cout << "ehPouso: " << avioesQueue.top()->ehPouso << endl;
            if (avioesQueue.top()->ehPouso) cout << "combustivel: " << avioesQueue.top()->combustivel << endl;
            else cout << "tempoVoo: " << avioesQueue.top()->tempoVoo << endl;
            cout << endl;

            for (int i = 0; i < 3; i++) {
                if (pistas[i].livre == 1) {
                    if (i == 0 || i == 1 || (i == 2 && !avioesQueue.top()->ehPouso)) {
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
            if (avioesQueue.top()->ehPouso) cout << "combustivel: " << avioesQueue.top()->contador << endl;
            else cout << "tempoVoo: " << avioesQueue.top()->contador << endl;
            cout << endl;

            avioesQueue.top()->naQueue = false;
            avioesQueue.pop();
            numEmergenciasTerminadas++;
        }
        // for (Aviao* it : avioesPouso) {
        //     if (it->naQueue && it->prioridade >= 2){
        //         avioesQueue.top()->naQueue = false;
        //     }
        // }
        // for (Aviao* it : avioesDecolagem) {
        //     if (it->naQueue && it->prioridade >= 2){
        //         avioesQueue.top()->naQueue = false;
        //     }
        // }

        // relatório da rodada
        cout << "Relatório da rodada:" << endl;
        cout << "Pistas: " << pistas[0].livre << " | " << pistas[1].livre << " | " << pistas[2].livre << endl;
        cout << endl;
        for (Aviao* it : avioesPouso) {
            if (it->naQueue) {
                cout << "Avião " << it->id << " esperando para pousar com combustível " << it->combustivel - it->contador << " e prioridade " << it->prioridade << endl;
                qtdeMediaCombustivelEsperandoPousar += it->combustivel - it->contador; 
            } else {
                tempoMedioEsperaPouso += it->contador;
                qtdeMediaCombustivelPousaram += it->combustivel;
            }
        }
        for (Aviao* it : avioesDecolagem) {
            if (it->naQueue){
                cout << "Avião " << it->id << " esperando para decolar com tempo de voo " << it->tempoVoo << " e prioridade " << it->prioridade << endl;
            } else {
                tempoMedioEsperaDecolagem += it->contador;
            }
        }
        cout << endl;
        cout << "Tempo médio de espera para pousar: " << tempoMedioEsperaPouso/avioesPouso.size() << endl;
        cout << "Tempo médio de espera para decolar: " << tempoMedioEsperaDecolagem/avioesDecolagem.size() << endl;
        cout << "Quantidade média de combustível dos que ainda não pousaram: " << qtdeMediaCombustivelEsperandoPousar/avioesPouso.size() << endl;
        cout << "Quantidade média de combustível dos que já pousaram: " << qtdeMediaCombustivelPousaram/avioesPouso.size() << endl;
        cout << "Quantidade de emergências finalizadas: " << numEmergenciasTerminadas << endl;
        cout << endl << "----------------------------------------" << endl << endl;

        // atualizar valores de combustível e cooldown
        for (int i = 0; i < 3; i++) {
            if (pistas[i].livre < 1) pistas[i].livre++;
        }
        for (Aviao* it : avioesPouso) {
            if (it->naQueue) it->contador++;
        } 
        for (Aviao* it : avioesDecolagem) {
            if (it->naQueue) it->contador++;
        } 

    }

    return 0;
}