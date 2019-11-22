#include<iostream>
#include<string>
#include<map>
#include<cstdlib>
#include "funcoesauxiliares.h"
#include "Indiceinvertido.h"
#include "leitor_arquivos.h"

using namespace std;

void processos(int n,indiceInvertido &in)
{
    system("cls");
    if(n==1)
    {
        string s;
        cout<<"Entre o nome do arquivo: ";
        cin>>s;
        in.addArquivo(s);
        in.criarIndice();
    }
    else if(n==2)
    {
        string s;
        cout<<"Entre o nome do arquivo: ";
        cin>>s;
        in.removerArquivo(s);
        in.criarIndice();
    }
    else if(n==3)
    {
        listarArquivos();
    }
    else if(n==4)
    {
        string s;
        cout<<"Digite os termos da busca: "<<endl;
        cin>>s;
        in.buscador(s);
    }
    else if(n==5)
    {
        lerArquivos(in);
        in.criarIndice();
    }
}




int main(){
    indiceInvertido in;
    int n;
    char c;
    cout << "O que voce deseja fazer?" << endl;
    do{
        cout << "Adicionar arquivo (1)" << endl
        << "Remover arquivo (2)" << endl<< "Consultar arquivos do indice (3)" << endl
        << "Fazer uma busca (ATENCAO: O INDICE NAO PODE ESTAR VAZIO) (4)" << endl
        << "Adicionar os arquivos padrao (5)" << endl << "Sair (6): ";
        cin>>n;
        if(n==6)
        {
            break;
        }
        processos(n,in);
        fflush(stdin);
        cout<<"Continuar(s/n)?";
        cin>>c;
        system("cls");
    }while(c!='n' && c!='N');
    return 0;
}


