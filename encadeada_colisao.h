#include <iostream>
#include <fstream>
#include <string>
#define SIZEOFDADOS sizeof(Dados)
#define SIZEOFBLOCO 4096


using namespace std;

//Tipo DADOS que será usado durante
class Dados {
    public:

        int  ID;
        string Titulo;
        int  Ano;
        string Autores;
        int  Citacoes;
        string Atualizacao;
        string Snippet;
        Dados(int id, string titulo, int ano, string autores, int citacoes, string atualizacao, string snipet){
            ID = id;
            Titulo = titulo; //trunca para caber no campo Titulo
            Ano = ano;
            Autores = autores; //trunca para caber no campo Autores
            Citacoes = citacoes;
            Atualizacao = atualizacao; //trunca para caber no campo Atualizacao
            Snippet = snippet; //trunca para caber no campo Snippet
        };
        //   Função que imprime um Dados da lista encadeada.
        
        void imprime(){
            printf("\n  Id: %d\n",ID);
            printf("  Ano: %d\n",Ano);
            printf("  Autores: %s\n",Autores.c_str());
            printf("  Citacoes:%d\n",Citacoes);
            printf("  Titulo: %s\n",Titulo.c_str());
            printf("  Atualizacao: %s\n",Atualizacao.c_str());
            printf("  Snipet: %s\n",Snippet.c_str());
        };
        ~Dados();

};

class Bloco {
    public:

        int fatorBloco;
        int ocupacao;
        void* *guardaDados; //vetor para guardar os elementos de dados
        Bloco* prox; // ponteiro para o bloco de Overflow
        Bloco(){
            fatorBloco = SIZEOFBLOCO/(SIZEOFDADOS);
            guardaDados = (void**) malloc(sizeof(void*)*fatorBloco);
            prox = NULL;
            ocupacao = 0;
            for(int i=0; i< fatorBloco;i++){ //limpa lixo de memoria
                guardaDados[i] = NULL;
            }
        }
        ~Bloco();
        void insere(Dados* NovoDados){
            guardaDados[ocupacao] = NovoDados;
            ocupacao = ocupacao+1;
        };
        void imprime(){
            for(int i = 0; i < ocupacao; i++){
                Dados* Atual = (Dados*) guardaDados[i];
                Atual->imprime();
            }
        };
        void* busca(int valor){
            Dados* Aux = (Dados*) guardaDados[0];
            Dados* Encontrado = NULL;
            if(Aux){
                for(int i = 0; i < ocupacao; i++){
                    Dados* Aux = (Dados*) guardaDados[i];
                    if(Aux->ID == valor){
                        Encontrado = Aux;
                    }
                }
            }
            return Encontrado;
        };
};

class Bucket {
     public:

        Bloco* Prim;
        int NumeroBlocos;
        int chave;
        void insere(Dados* NovoDados);
        Bloco* removee();
        void* busca(int valor);
        void imprime();
        Bucket(int Chave){
            chave = Chave;
            Prim = NULL;
            NumeroBlocos = 0;
        };
        ~Bucket();

};

void Bucket::insere(Dados* NovoElem){
    Bloco* Aux = Prim;
    bool inserido = false;
   
    if(Prim == NULL){ // Se o bucket tá vazio, cria um bloco
        Prim = new Bloco();
        Prim->insere(NovoElem);
        NumeroBlocos = NumeroBlocos+1;
        inserido = true;
    }
    while(!inserido){
        if(Aux->ocupacao < Aux->fatorBloco){ // verifica se tem espaco no bloco, se tiver, insere os dados no bloco
            Aux->insere(NovoElem);
            inserido = true;
        }
        if(Aux->prox == NULL && !inserido){
            Aux->prox = new Bloco();
            NumeroBlocos = NumeroBlocos+1;
        }
        Aux = Aux->prox;
    }
};

void Bucket::imprime(){

    Bloco* Aux = Prim;
    while(Aux){
        Aux->imprime();
        Aux = Aux->prox;
    }
};

void* Bucket::busca(int valor){
    Bloco* Aux = (Bloco*) Prim;
    Dados* Encontrado = NULL;
    bool achou = false;
    while(!achou && Aux){
        Encontrado = (Dados*) Aux->busca(valor);
        if(Encontrado){
            achou = true;
        }
        Aux = Aux->prox;
    }
    return Encontrado;
};