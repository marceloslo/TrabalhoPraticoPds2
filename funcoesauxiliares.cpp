#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Indiceinvertido.h"

using namespace std;

vector<string> separarPalavras(string s)
{
    string palavra;
    vector<string> termos;
    s=converteTexto(s);
    for(string::iterator it=s.begin();it!=s.end();it++)
    {
        if(*it==' ')
        {
            termos.push_back(palavra);
            palavra="";
        }
        else if(it==s.end()-1)
        {
            palavra+=*it;
            termos.push_back(palavra);
        }
        else
        {
            palavra+=*it;
        }
    }
    return termos;
}

string converteTexto(string s)
{
    string convertido;
    for(string::iterator it=s.begin();it!=s.end();it++)
    {
        if(isdigit(*it)||isalpha(*it)||(*it)==' ')
        {
            convertido.push_back(tolower(*it));
        }
    }
    return convertido;
}

void display(vector<string> vetor)
{
    unsigned int i;
    cout<<"Posicao\tDocumentos"<<endl;
    for(i=0;i<vetor.size();i++)
    {
        cout<<i+1<<"\t"<<vetor[i]<<endl;
    }
}

bool checarRepeticao(string s)
{
    ifstream arq;
    string texto;
    arq.open("enderecos.txt");
    while(arq >> texto)
    {
        if(texto==s)
        {
            return true;
        }
    }
    return false;
}
void listarArquivos()
{
    ifstream arquivos;
    string s;
    arquivos.open("enderecos.txt");
    while(getline(arquivos,s))
    {
        cout<<s<<endl;
    }
    return;
}
