#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include "Indiceinvertido.h"

using namespace std;

//construtor que cria os arquivos

indiceInvertido::indiceInvertido()//construtor usando os arquivos
{
    ifstream arquivo;
    string s, palavraAtual;
    bool b = true;
    arquivo.open("indice.txt");
    if(!arquivo.is_open())
    {
       this->criarIndice();
    }
    else
    {
        nPalavras=0;
        nArquivos=0;
        while(getline(arquivo, s)){
            if(b){
                palavraAtual = s;
                b = false;
                continue;
            }
            else if(s.compare("!") == 0){
                b = true;
                continue;
            }
            arqPalavras[palavraAtual].insert(s);
        }
        lerFrequencia();
        nPalavras=arqPalavras.size();
        arquivo.close();
        arquivo.open("enderecos.txt");
        while(getline(arquivo,s))
            {
                nArquivos++;
            }
        arquivo.close();
    }
}

void indiceInvertido::lerFrequencia()//le o arquivo de frequencia e salva no map frequencias
{
    ifstream freq;
    string s, documentoAtual, palavraAtual;
    bool linhaDocumento = true, linhaPalavra = false;
    freq.open("freq.txt");
    while(getline(freq,s))
    {
        if(linhaDocumento){
            documentoAtual = s;
            linhaDocumento = false;
            linhaPalavra = true;
            continue;
        }
        else if(s.compare("!") == 0){
            linhaDocumento = true;
            continue;
        }
        else if(linhaPalavra){
            palavraAtual = s;
            linhaPalavra = false;
            continue;
        }
        frequencia[documentoAtual][palavraAtual] = atoi(s.c_str());
        linhaPalavra = true;
    }
    freq.close();
}

void indiceInvertido::criarIndice()
{
    arqPalavras.clear();
    frequencia.clear();
    nArquivos=0;
    nPalavras=0;
    set<string> aux;
    ifstream arq;
    string s;
    arq.open("enderecos.txt");
    if(arq.is_open())
    {
        while(getline(arq,s))
        {
            aux=this->acessarArquivos(s);
            for(set<string>::iterator it=aux.begin();it!=aux.end();it++)
            {
                arqPalavras[*it].insert(s);
            }
            nArquivos++;
        }
        nPalavras=arqPalavras.size();
        arq.close();
    }
    this->criarArquivo();
}


set<string> indiceInvertido::acessarArquivos(string s)//acessa um arquivo salvo em enderecos e cria o mapa frequencia e retorna o set
{
    ifstream arquivo;
    arquivo.open(s.c_str());
    string texto;
    set<string> palavras;
    if(!arquivo.is_open())
    {
        return palavras;
    }
    while(arquivo >> texto)
    {
        texto=converteTexto(texto);
        palavras.insert(texto);
        frequencia[s][texto]=frequencia[s][texto] + 1;
    }
    arquivo.close();
    return palavras;
}

void indiceInvertido::criarArquivo(){//transforma arq palavras(o indice invertido) em arquivo
    ofstream s;
    s.open("indice.txt");
    for(map<string, set<string> >::iterator it = arqPalavras.begin(); it != arqPalavras.end(); it++){
        s<<it->first<<"\n";
        for(set<string>::iterator i = it->second.begin(); i != it->second.end(); i++){
            s<<*i <<"\n";
        }
        s<<"!\n";
    }
    this->criarFrequencia();
    s.close();
}

void indiceInvertido::criarFrequencia()//transforma o mapa de frequencia em um arquivo(que sera lido por ler frequencia)
{
    ofstream freq;
    freq.open("freq.txt");
    for(map<string, map<string, int> >::iterator it=frequencia.begin();it!=frequencia.end();it++)
    {
        freq << it->first << "\n";
        for(map<string, int>::iterator i=it->second.begin();i!=it->second.end();i++)
        {
            freq<<i->first<<"\n";
            freq<<i->second<<"\n";
        }
        freq<<"!\n";
    }
    freq.close();
}

void indiceInvertido::addArquivo(string s)//adiciona em endereços.txt
{
    if(checarRepeticao(s))
    {
        return;
    }
    FILE *arq;
    arq = fopen("enderecos.txt","a");
    if(arq==NULL){
        arq = fopen("enderecos.txt","w");
    }
    fprintf(arq,"%s\n",s.c_str());
    fclose(arq);
}

void indiceInvertido::removerArquivo(string s)//remove de endereços.txt
{
    string teste;
    ofstream substituto;
    ifstream arq;
    substituto.open("sub.txt");
    arq.open("enderecos.txt");
    while(arq>>teste)
    {
        if(s!=teste)
        {
           substituto<<teste<<"\n";
        }
    }
    substituto.close();
    arq.close();
    remove("enderecos.txt");
    rename("sub.txt","enderecos.txt");
}


void indiceInvertido::buscador(string s)//A função principal da classe, que irá realizar as buscas necessárias e imprimir na tela depois
{
    map<string,double> Wq;
    map<string,double> sim;
    vector<string> results;
    findW();
    findWq(s,Wq);
    sim=findCos(Wq);
    results=classificacao(sim);
    display(results);
    coordenadas.clear();
}

vector<string> indiceInvertido::classificacao(map<string,double> sim)//Essa função é responsável por construir a classificação dos documentos
{
    unsigned int i=0,j;
    double aux;
    string buf;
    vector<double> valor;
    vector<string> ordem;
    for(map<string,double>::iterator it=sim.begin();it!=sim.end();it++)
    {
        ordem.push_back(it->first);
        valor.push_back(it->second);
        i++;
    }
    for(i=0;i<valor.size();i++)
    {
        for(j=i+1;j<valor.size();j++)
        {
            if(valor[j]>valor[i])
            {
                aux=valor[j];
                valor[j]=valor[i];
                valor[i]=aux;
                buf=ordem[j];
                ordem[j]=ordem[i];
                ordem[i]=buf;
            }
        }
    }
    for(i=0;i<valor.size()-1;i++)
    {
        if(valor[i]==valor[i+1])
        {
            ordem[i]+=" e ";
            ordem[i]+=ordem[i+1];
            ordem.erase(ordem.begin()+i+1);
            valor.erase(valor.begin()+i+1);
            i=i-1;
        }
    }
    return ordem;
}

map<string,double> indiceInvertido::findCos(map<string,double> Wq)//essa função acha a similaridade de acordo com a formula
{
    ifstream documentos;
    string dj;
    map<string,double> sim;
    double E_dj_q,E_dj,E_q;
    documentos.open("enderecos.txt");
    while(getline(documentos,dj))
    {
        E_dj=0;
        E_q=0;
        E_dj_q=0;
        for(map<string, set<string> >::iterator pi=arqPalavras.begin();pi!=arqPalavras.end();pi++)
        {
            E_dj_q=E_dj_q+(coordenadas[dj][pi->first]*Wq[pi->first]);
            E_dj=E_dj+(coordenadas[dj][pi->first]*coordenadas[dj][pi->first]);
            E_q=E_q+(Wq[pi->first]*Wq[pi->first]);
        }
        coordenadas.erase(dj);
        sim[dj]= E_dj_q/sqrt(E_dj*E_q);
    }
    documentos.close();
    return sim;
}

void indiceInvertido::findWq(string s,map<string,double> &Wq)//essa função acha as cordenadas das palavras da busca
{
    vector<string> termos;
    termos=separarPalavras(s);
    for(vector<string>::iterator px=termos.begin();px!=termos.end();px++)
    {
        Wq[*px]=(findIdf(*px)*count(termos.begin(),termos.end(),*px));
    }

}

void indiceInvertido::findW()//Acha as coordenadas de todas palavras
{
    string dj;
    for(map<string, set<string>>::iterator px=arqPalavras.begin();px!=arqPalavras.end();px++)
    {
        for(set<string>::iterator dj=px->second.begin();dj!=px->second.end();dj++)
            {
                coordenadas[*dj][px->first]=(findIdf(px->first) * findTf(px->first,*dj));
            }
    }
}


double indiceInvertido::findIdf(string s)// essa função calcula o idf de uma dada palavra
{
    double nx;
    nx=arqPalavras[s].size();
    if(nx==0)
    {
        return 0;
    }
    return log(nArquivos/nx);
}

int indiceInvertido::findTf(string px,string dj)// essa função obtem a frequencia da palavra
{
    return frequencia[dj][px];
}
