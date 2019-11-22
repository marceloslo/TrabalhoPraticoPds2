#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
//funções auxiliares
    // separa as palavras,ja convertendo-as e armazena cada uma em uma posição do vector
    vector<string> separarPalavras(string s);
    //converte o texto, mantendo apenas digitos e numeros
    string converteTexto(string s);
    //imprime na tela os conteudos do vetor
    void display(vector<string> vetor);
    //Checa se tem algum arquivo de nome s em endereços.txt
    bool checarRepeticao(string s);
    //lista todos arquivos de enderecos.txt
    void listarArquivos();

#endif // FUNCOESAUXILIARES
