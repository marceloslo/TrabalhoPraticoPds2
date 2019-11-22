#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include "funcoesauxiliares.h"
#include "Indiceinvertido.h"
#include "doctest.h"

using namespace std;

class Teste
{
public:
    static int NumArquivos(const indiceInvertido &in)
    {
        return in.nArquivos;
    }
    static int NumPalavras(const indiceInvertido &in)
    {
        return in.nPalavras;
    }
    static map<string,set<string> > PalavrasDocumentos(const indiceInvertido &in)
    {
        return in.arqPalavras;
    }
    static map<string, map<string, int> > frequencias(const indiceInvertido &in)
    {
        return in.frequencia;
    }
    static int Tf(indiceInvertido &in,string px,string dj)
    {
        return in.findTf(px,dj);
    }
    static double Idf(indiceInvertido &in,string s)
    {
        return in.findIdf(s);
    }
    static map<string,map<string,double> > W(indiceInvertido &in)
    {
        in.findW();
        return in.coordenadas;
    }
    static void Wq(indiceInvertido &in,string s,map<string,double> &Wq)
    {
        in.findWq(s,Wq);
    }
    static map<string,double> Cosseno(indiceInvertido &in,map<string,double> Wq)
    {
        return in.findCos(Wq);
    }
    static vector<string> Classifica(indiceInvertido &in,map<string,double> sim)
    {
        return in.classificacao(sim);
    }
};

TEST_SUITE("Funções relacionadas ao arquivo que guarda endereços")
{
    TEST_CASE("addArquivo")
    {
        remove("enderecos.txt");
        ofstream af,bf,cf,df;
        int i=0;
        af.open("a.txt");
        bf.open("b.txt");
        cf.open("c.txt");
        df.open("d.txt");
        af.close();
        bf.close();
        cf.close();
        df.close();//até aqui, estão apenas sendo criados arquivos que serão usados para os testes.
        ifstream enderecos;
        indiceInvertido in;
        string s;
        in.addArquivo("a.txt");
        in.addArquivo("b.txt");
        in.addArquivo("c.txt");
        in.addArquivo("d.txt");
        enderecos.open("enderecos.txt");
        while(enderecos>>s)
        {
            i++;
            if(i==1)
            {
                REQUIRE(s=="a.txt");
            }
            else if(i==2)
            {
                CHECK(s=="b.txt");
            }
            else if(i==3)
            {
                CHECK(s=="c.txt");
            }
            else
            {
                CHECK(s=="d.txt");
            }
        }
        enderecos.close();
        remove("enderecos.txt");//para evitar testes posteriores considerem arquivos de testes anteriores
    }
    TEST_CASE("removerArquivo")
    {
        ifstream enderecos;
        string s;
        indiceInvertido in;
        in.addArquivo("a.txt");
        in.addArquivo("b.txt");
        in.addArquivo("c.txt");
        in.addArquivo("d.txt");
        in.removerArquivo("a.txt");
        in.removerArquivo("b.txt");
        enderecos.open("enderecos.txt");
        while(enderecos>>s)
        {
            REQUIRE(s!="a.txt");
            CHECK(s!="b.txt");
        }
        enderecos.close();
        remove("enderecos.txt");
    }
    TEST_CASE("nArquivos")
    {
        indiceInvertido in;
        REQUIRE(Teste::NumArquivos(in)==0);
        in.addArquivo("a.txt");
        in.criarIndice();
        CHECK(Teste::NumArquivos(in)==1);
        in.addArquivo("b.txt");
        in.criarIndice();
        CHECK(Teste::NumArquivos(in)==2);
        in.addArquivo("c.txt");
        in.criarIndice();
        CHECK(Teste::NumArquivos(in)==3);
        in.addArquivo("d.txt");
        in.criarIndice();
        CHECK(Teste::NumArquivos(in)==4);
        remove("enderecos.txt");
    }
    TEST_CASE("addArquivo,checar repeticao e arquivos repetidos")
    {
        string s;
        int i=0;
        ifstream enderecos;
        indiceInvertido in;
        in.addArquivo("a.txt");
        in.addArquivo("a.txt");
        in.addArquivo("a.txt");
        in.criarIndice();
        enderecos.open("enderecos.txt");
        while(getline(enderecos,s))
        {
            i++;
        }
        CHECK(i==1);//mostra que só tem uma linha com texto no arquivo, no caso, a.txt
        CHECK(Teste::NumArquivos(in)==1);
        enderecos.close();
        remove("enderecos.txt");
    }
}

TEST_SUITE("Elementos responsáveis por ler arquivos inseridos pelo usuario")
{
    TEST_CASE("converteTexto")
    {
        indiceInvertido in;
        string s;
        s=converteTexto("Teste-ConVerte?::");
        CHECK(s=="testeconverte");
        s=converteTexto("1[2´34_5=6?7/8");
        CHECK(s=="12345678");
    }
    TEST_CASE("arqPalavras")//guarda todos os documentos em que uma palavra está presente
    {
        ofstream a,b;
        map<string,set<string> > testeAcesso;
        indiceInvertido in;
        a.open("a.txt");
        b.open("b.txt");
        b<<"Testando teste";
        a<<"Testar teste Teste";
        a.close();
        b.close();
        in.addArquivo("a.txt");
        in.addArquivo("b.txt");
        in.criarIndice();
        testeAcesso=Teste::PalavrasDocumentos(in);
        CHECK(testeAcesso["teste"].count("a.txt")==1);
        CHECK(testeAcesso["Teste"].count("a.txt")==0);
        CHECK(testeAcesso["teste"].count("b.txt")==1);
        CHECK(testeAcesso["testando"].count("a.txt")==0);
        CHECK(testeAcesso["testando"].count("b.txt")==1);
        remove("enderecos.txt");
    }
    TEST_CASE("acessar arquivos")//se a função é funcional,arqPalavras[documento] deve conter uma copia de cada palavra do documento.
    {
        ofstream a;
        map<string,set<string> > testeAcesso;
        indiceInvertido in;
        a.open("a.txt");
        a<<"Isso deve estar em arqpalavras associado a a.txt";
        a.close();
        in.addArquivo("a.txt");
        in.criarIndice();
        testeAcesso=Teste::PalavrasDocumentos(in);
        CHECK(testeAcesso["isso"].count("a.txt")==1);
        CHECK(testeAcesso["deve"].count("a.txt")==1);
        CHECK(testeAcesso["estar"].count("a.txt")==1);
        CHECK(testeAcesso["em"].count("a.txt")==1);
        CHECK(testeAcesso["arqpalavras"].count("a.txt")==1);
        CHECK(testeAcesso["associado"].count("a.txt")==1);
        CHECK(testeAcesso["a"].count("a.txt")==1);
        CHECK(testeAcesso["atxt"].count("a.txt")==1);
        remove("enderecos.txt");
    }
    TEST_CASE("nPalavras")
    {
        ofstream a;
        indiceInvertido in;
        a.open("a.txt");
        a<<"Existem cinco palavras em a";
        a.close();
        in.addArquivo("a.txt");
        in.criarIndice();
        CHECK(Teste::NumPalavras(in)==5);
        in.removerArquivo("a.txt");
        a.open("a.txt");
        a<<"Agora existem seis palavras em a";
        a.close();
        in.addArquivo("a.txt");
        in.criarIndice();
        CHECK(Teste::NumPalavras(in)==6);
        remove("enderecos.txt");
    }
    TEST_CASE("frequencia")
    {
        ofstream a,b;
        indiceInvertido in;
        a.open("a.txt");
        a<<"oi oi oi oi oi aparece cinco vezes em a";
        b.open("b.txt");
        b<<"oi oi aparece duas vezes em b";
        a.close();
        b.close();
        in.addArquivo("a.txt");
        in.addArquivo("b.txt");
        in.criarIndice();
        CHECK(Teste::frequencias(in)["a.txt"]["oi"]==5);
        CHECK(Teste::frequencias(in)["b.txt"]["oi"]==2);
        CHECK(Teste::frequencias(in)["a.txt"]["em"]==1);
        remove("enderecos.txt");
    }
}

TEST_SUITE("Funcoes que criam e leem arquivos")
{
    TEST_CASE("criarFrequencia")
    {
        indiceInvertido in;
        int i;
        bool testeA=false,testeB=false;
        string s,passado;
        ifstream t;
        ofstream a,b;
        a.open("a.txt");
        a<<"A a a a a aparece 5 vezes";
        a.close();
        in.addArquivo("a.txt");
        in.criarIndice();
        t.open("freq.txt");
        while(t>>s)// testa se a tem frequencia 5 com apenas a.txt
        {
            if(s=="a")
            {
                passado=s;
                testeA=true;
            }
            else if(testeA==true)
            {
                i=stoi(s);
                CHECK(i==5);
                testeA=false;
            }
        }
        b.open("b.txt");
        b<<"A a a aparece 3 vezes";
        b.close();
        in.addArquivo("b.txt");
        in.criarIndice();
        while(t>>s)//testa se a tem frequencia 5 associada a a.txt e 3 associada a b.txt de acordo com o arquivo.
        {
            if(s=="b.txt")
            {
                testeB=true;
            }
            else if(s=="a")
            {
                passado=s;
                testeA=true;
            }
            else if(testeA==true && testeB==false)
            {
                i=stoi(s);
                CHECK(i==5);
                testeA=false;
            }
            else if(testeA==true && testeB==true)
            {
                i=stoi(s);
                CHECK(i==3);
                testeA=false;
            }
        }
        remove("enderecos.txt");
    }
    TEST_CASE("criarArquivo")
    {
        indiceInvertido in;
        ofstream a,b,c;
        string s,passado;
        ifstream t;
        a.open("a.txt");
        b.open("b.txt");
        c.open("c.txt");
        a << "a esta em a";
        b << "a esta em b";
        c << "nao esta em c";
        a.close();
        b.close();
        c.close();
        in.addArquivo("a.txt");
        in.addArquivo("b.txt");
        in.addArquivo("c.txt");
        in.criarIndice();
        t.open("indice.txt");
        while(t>>s)
        {
            if(s=="a"||s=="!")
            {
                passado=s;
            }
            else if(passado=="a")
            {
                CHECK(s!="c.txt");
            }
            else if(passado=="!")
            {
                CHECK(s!="a.txt");
                CHECK(s!="b.txt");
                CHECK(s!="c.txt");
                passado=s;
            }
        }
        t.close();
        remove("enderecos.txt");
    }
    TEST_CASE("lerFrequencia")
    {
        ofstream a,b;
        ifstream t;
        string s,arquivo,passado;
        int freqA=0,freqB=0;
        indiceInvertido in;
        a.open("a.txt");
        a<<"oi oi oi oi oi aparece cinco vezes em a";
        b.open("b.txt");
        b<<"oi oi aparece duas vezes em b";
        a.close();
        b.close();
        in.addArquivo("a.txt");
        in.addArquivo("b.txt");
        in.criarIndice();
        t.open("freq.txt");
        while(t>>s)
        {
            if(s=="a.txt"||s=="b.txt")
            {
                arquivo=s;
            }
            else if(arquivo=="a.txt" && passado=="oi")
            {
                freqA=stoi(s);
            }
            else if(arquivo=="b.txt" && passado=="oi")
            {
                freqB=stoi(s);
            }
            passado=s;
        }
        t.close();
        REQUIRE(freqA==5);
        REQUIRE(freqB==2);
        CHECK(Teste::frequencias(in)["a.txt"]["oi"]==freqA);
        CHECK(Teste::frequencias(in)["b.txt"]["oi"]==freqB);
        remove("enderecos.txt");
    }
    TEST_CASE("criarIndice")//é chamado pela função (ou no construtor caso indice.txt nao exista)
    {
        ofstream a,b;
        indiceInvertido in;
        a.open("a.txt");
        b.open("b.txt");
        a<<"Teste criar indice";
        b<<"Deve ter essas palavras com frequencia um";
        a.close();
        b.close();
        in.addArquivo("a.txt");
        in.addArquivo("b.txt");
        in.criarIndice();
        CHECK(Teste::PalavrasDocumentos(in)["criar"].count("a.txt")==1);
        CHECK(Teste::PalavrasDocumentos(in)["indice"].count("a.txt")==1);
        CHECK(Teste::PalavrasDocumentos(in)["teste"].count("a.txt")==1);
        CHECK(Teste::frequencias(in)["b.txt"]["deve"]==1);
        CHECK(Teste::frequencias(in)["b.txt"]["ter"]==1);
        CHECK(Teste::frequencias(in)["b.txt"]["essas"]==1);
        CHECK(Teste::frequencias(in)["b.txt"]["palavras"]==1);
        CHECK(Teste::NumPalavras(in)==10);
        CHECK(Teste::NumArquivos(in)==2);
        remove("enderecos.txt");
        remove("indice.txt");
    }
    TEST_CASE("indiceInvertido()")
    {
        indiceInvertido in;
        CHECK(Teste::NumArquivos(in)==0);
        CHECK(Teste::NumPalavras(in)==0);
        CHECK(Teste::frequencias(in).empty());
        CHECK(Teste::PalavrasDocumentos(in).empty());
    }
}

TEST_SUITE("Funções relacionadas a classificação")
{
    TEST_CASE("findTf")
    {
        indiceInvertido in;
        ofstream a;
        a.open("a.txt");
        a<<"Tf de t e 5 t t t t";
        a.close();
        in.addArquivo("a.txt");
        in.criarIndice();
        REQUIRE(Teste::Tf(in,"t","a.txt")==5);
        CHECK(Teste::Tf(in,"5","a.txt")==1);
        remove("enderecos.txt");
    }
    TEST_CASE("findIdf")
    {
        indiceInvertido in;
        ofstream a,b;
        a.open("a.txt");
        b.open("b.txt");
        a<<"Teste a";
        b<<"teste b";
        a.close();
        b.close();
        in.addArquivo("a.txt");
        in.addArquivo("b.txt");
        in.criarIndice();
        CHECK(Teste::Idf(in,"teste")==log(1));
        CHECK(Teste::Idf(in,"a")==log(2/1));
        CHECK(Teste::Idf(in,"b")==log(2/1));
        remove("enderecos.txt");
    }
    TEST_CASE("findW e coordenadas")
    {
        ofstream a;
        indiceInvertido in;
        map<string,map<string, double> > t;
        a.open("a.txt");
        a<<"A a a a a tem w = log(2/1)*5";
        a.close();
        in.addArquivo("a.txt");
        in.addArquivo("d.txt");
        in.criarIndice();
        t=Teste::W(in);
        t=Teste::W(in);
        CHECK(t["a.txt"]["a"]==log(2/1)*5);
        CHECK(t["a.txt"]["tem"]==log(2/1));
        remove("enderecos.txt");
    }
    TEST_CASE("findWq")
    {
        indiceInvertido in;
        ofstream a;
        string s="Wq de a a a a = log(2/1)*4";
        map<string,double> t;
        a.open("a.txt");
        a<<"A a a a a tem w = log(2/1)*5";
        a.close();
        in.addArquivo("a.txt");
        in.addArquivo("d.txt");
        in.criarIndice();
        Teste::Wq(in,s,t);
        CHECK(t["a"]==log(2/1)*4);
        CHECK(t["Wq"]==0);//nao esta presente nos arquivos
        CHECK(t["W"]==0);//frequencia 0 na string
        remove("enderecos.txt");
    }
    TEST_CASE("separarPalavras")
    {
        string s;
        indiceInvertido in;
        vector<string> separada;
        s="separada deve ter tamanho 5";
        separada=separarPalavras(s);
        REQUIRE(separada.size()==5);
        CHECK(separada[0]=="separada");
    }
    TEST_CASE("findCos")
    {
        indiceInvertido in;
        ofstream a;
        string s="a b";
        map<string,double> t,cossenos;
        map<string,set<string> > palavras;
        a.open("a.txt");
        a<<"A a a b";
        a.close();
        a.open("b.txt");
        a<<"A A C";
        a.close();
        a.open("c.txt");
        a<<"A A";
        a.close();
        a.open("d.txt");
        a<<"B B";
        a.close();
        in.addArquivo("a.txt");
        in.addArquivo("b.txt");
        in.addArquivo("c.txt");
        in.addArquivo("d.txt");
        in.criarIndice();
        palavras=Teste::PalavrasDocumentos(in);
        for (map<string,set<string> >::iterator it=palavras.begin();it!=palavras.end();it++)
        {
            Teste::W(in);
            t[it->first]=0;
        }
        Teste::Wq(in,s,t);
        cossenos=Teste::Cosseno(in,t);
        CHECK(int(cossenos["a.txt"]*100)==87);//checar se o valor inteiro + os 2 primeiros decimais são equivalentes ao esperado
        CHECK(int(cossenos["b.txt"]*100)==14);
        CHECK(int(cossenos["c.txt"]*100)==38);
        CHECK(int(cossenos["d.txt"]*100)==92);
        SUBCASE("Classificacao() e buscador")//se classificação,findw,findwq estão certos , buscador automaticamente esta.
        {
            vector<string> ordem;
            ordem = Teste::Classifica(in,cossenos);
            CHECK(ordem[0]=="d.txt");
            CHECK(ordem[1]=="a.txt");
            CHECK(ordem[2]=="c.txt");
            CHECK(ordem[3]=="b.txt");
        }
        remove("enderecos.txt");
    }
}
