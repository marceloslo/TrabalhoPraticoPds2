#include "leitor_arquivos.h"
#include "Indiceinvertido.h"
#include <dirent.h>
#include <string>

using namespace std;

void lerArquivos(indiceInvertido i){
    string dir = ".\\20_newsgroups";
    string diratual;
    DIR *p_principal = nullptr;
    DIR *p_secundario = nullptr;
    struct dirent *pastas = nullptr;
    struct dirent *arquivos = nullptr;
    p_principal = opendir(dir.c_str());
    if(p_principal == nullptr) return;
    while((pastas = readdir(p_principal))){
        if(pastas == nullptr) continue;
        if(string(pastas->d_name).compare(".") == 0 || string(pastas->d_name).compare("..") == 0) continue;
        diratual = dir + "\\" + pastas->d_name;
        p_secundario = opendir(diratual.c_str());
        if(p_secundario == nullptr) continue;

        while((arquivos = readdir(p_secundario))){
            if(string(arquivos->d_name).compare(".") == 0 || string(arquivos->d_name).compare("..") == 0) continue;
            i.addArquivo(diratual + "\\" + arquivos->d_name);
        }
    }
    closedir(p_principal);
    closedir(p_secundario);
}
