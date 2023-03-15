// EP1 de MAC0323
// Nome: Gabriel Haruo Hanai Takeuchi
// NUSP: 13671636

#include <iostream>
#include <queue>

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
        Aviao(string id, string codigo, int combustivel, int tempoVoo){
            this->id = setId(id);
            this->codigo = setCodigo(codigo);
            this->combustivel = setCombustivel(combustivel);
            this->tempoVoo = setTempoVoo(tempoVoo);
        }

        // Metodos

        // Getters e Setters
        void setId() {

        }

        void setCodigo() {

        }
        
        void setCombustivel() {

        }

        void setTempoVoo() {

        }

};

int main() {
    // Construção das pistas

    // Construção dos aviões

    // Construção da fila de aviões
    queue<Aviao> fila;


    return 0;
}