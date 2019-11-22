#include <iostream>
#include <string>
#include <set>
#include<vector>
#include <map>
#include "funcoesauxiliares.h"
#ifndef indiceInvertido_H
#define indiceInvertido_H

using namespace std;
//funções auxiliares
    vector<string> separarPalavras(string s);// separa as palavras,ja convertendo-as e armazena cada uma em uma posição do vector
    string converteTexto(string s);//converte o texto, mantendo apenas digitos e numeros
    void display(vector<string> vetor);//imprime na tela os conteudos do vetor
    bool checarRepeticao(string s);//Checa se tem algum arquivo de nome s em endereços.txt

class indiceInvertido
{
public:
    //construtor da classe, jс acessa todos arquivos anotados no arquivo enderecos.txt e indexa eles em tempo de execuчуo
    indiceInvertido();
    //realiza a busca dos termos desejados
    void buscador(string s);
    //adiciona um novo arquivo que pode ser indexado por meio de enderecos.txt
    void addArquivo(string s);
    //remove um arquivo da indexação por meio de enderecos.txt
    void removerArquivo(string s);
    //cria um indice
    //uso:É apenas necessário usar essa função se você adicionou ou removeu algum arquivo após a ultima execução do programa, caso contrario o indice ja sera criado automaticamente com o construtor
    //prerequisito:Foi adicionado pelo menos um arquivo(que existe) em alguma execução do indice.
    void criarIndice();

    friend class Teste;
private:

    void lerFrequencia();
    set<string> acessarArquivos(string s);
    void criarArquivo();
    void criarFrequencia();
    vector<string> classificacao(map<string,double> sim);
    map<string,double> findCos(map<string,double> Wq);
    void findWq(string s,map<string,double> &Wq);
    void findW();
    double findIdf(string s);
    int findTf(string px,string dj);

    map<string,map<string,double> > coordenadas;//documento->palavra->coordenada
    map<string, set<string> > arqPalavras;//palavra->documento
    map<string, map<string, int> > frequencia;//documento->palavra->numero de vezes
    int nPalavras;
    int nArquivos;
};

#endif // indiceInvertido_H



























