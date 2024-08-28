#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include "arquivosAleatorios.hpp"


void geraAleatorios(int qtddNumeros, ofstream&  arquivo, int i){
    for(int i = 0 ; i < qtddNumeros ; i++){
        arquivo << rand() << endl; //se 1+rand()%10 = numeros de 1 a 10
    }
}

void criarArquivos(int qtddArquivos, int qtddNumeros) {

    for (int i = 1; i <= qtddArquivos; ++i) {
        string nomeArquivo = "arq/arquivo" + to_string(i) + ".txt";

        ofstream arquivo(nomeArquivo); // Abrindo o arquivo para escrita
        
        if (arquivo.is_open()) { // Verificando se o arquivo foi aberto com sucesso
            geraAleatorios(qtddNumeros, arquivo, i); //preenchendo o arquivo
            arquivo.close();
        } else {
            cout << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        }
    }
}

// Função para ler dados dos arquivos e salvar em vetores
vector<int> leArquivo(string nomeArquivo) {

    vector<int> dados;

    ifstream arquivo(nomeArquivo); // Abrindo o arquivo para leitura
    
    if (arquivo.is_open()) { // Verificando se o arquivo foi aberto com sucesso
        int numero;
        while (arquivo >> numero) { // Lendo os dados do arquivo e salvando no vetor
            dados.push_back(numero);
        }
        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo " << nomeArquivo << endl;
    }

    return dados;
}
