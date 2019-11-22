#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
//fun��es auxiliares
    // separa as palavras,ja convertendo-as e armazena cada uma em uma posi��o do vector
    vector<string> separarPalavras(string s);
    //converte o texto, mantendo apenas digitos e numeros
    string converteTexto(string s);
    //imprime na tela os conteudos do vetor
    void display(vector<string> vetor);
    //Checa se tem algum arquivo de nome s em endere�os.txt
    bool checarRepeticao(string s);
    //lista todos arquivos de enderecos.txt
    void listarArquivos();

#endif // FUNCOESAUXILIARES
