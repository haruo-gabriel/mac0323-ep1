// EP1 de MAC0323
// Nome: Gabriel Haruo Hanai Takeuchi
// NUSP: 13671636

#include <iostream>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <string>

using namespace std;

class Pista {
    private:
        int id;
        bool podeDecolar;
        bool podePousar;
        int livre;
        // A cada rodada, livre é incrementado.
        // A pista está livre se livre >= 1, e ocupada se livre <= 0.
        // Se for ocupada, imediatamente muda seu valor para -2 (cooldown).

    public:
        // Construtor
        Pista(int id, bool podeDecolar, bool podePousar) {
            this->id = setId(id);
            this->podeDecolar = setPodeDecolar(podeDecolar);
            this->podePousar = setPodePousar(podePousar);
            this->livre = true;
        }

        // Metodos

        // Getters e Setters
        void setId() {

        }

        void setPodeDecolar() {

        }
        
        void setPodePousar() {

        }

}

class Aviao {
    private:
        string id; // exemplo: go123 (companhia GOL, aviao 123)
        string codigo; // o codigo tem exatamente 3 letras - como otimizar o type do codigo?
        int combustivel;
        int tempoVoo;

    public:
        // Construtor
        Aviao(int combustivel, int tempoVoo){
            id = setId;
            this->combustivel = setCombustivel(combustivel);
            this->tempoVoo = setTempoVoo(tempoVoo);
        }

        // Metodos


        // Getters e Setters
        string setId(){
            string temp = rand()
        }

        void setCombustivel() {

        }

        void setTempoVoo() {

        }

};

int main() {
    int T, K, C, V;
    float probEmergencia, probPouso, probDecolagem;
    probDecolagem = 1.0 - probDecolagem;

    vector<Pista> pistas;
    Pista pista1(1, 1, 1);
    Pista pista2(2, 1, 1);
    Pista pista3(3, 1, 0);

    vector<Aviao> avioes;
    queue<Aviao*> fila;
    
    char Alphabets[26] = {'a','b','c','d','e','f','g','h', 'i','j','k','l','m','n','o','p','q', 'r','s','t','u','v','w','x','y','z'};

    // Entradas

    // Construção do algoritmo

    for (int t = 0; t < T; t++){
        int qtdAvioes = probAvioes(K);
        for (int i = 0; i < qtdAvioes; i++){
            Aviao aviao(C, V);
            // a quantidade de combustivel que um aviao chega no aeroporto é fixa?
            avioes.push_back(aviao);
        }

        for (avioes.begin())
        
        for (auto it = avioes.begin(); it != avioes.end(); it++){
            //decrementar combustivel
            //
        }

    }


    return 0;
}