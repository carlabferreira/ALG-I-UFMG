//---------------------------------------------------------------------
// Arquivo	: tp0.cpp
// Conteudo	: implementacao do TP0
// Autor	: Carla Beatriz Ferreira (2022097470)
// Historico	: 2024-03-22 - arquivo criado
//                2024-03-30 - edição do modo de armazenagem dos numeros aleatorios para ter
//                             50 arquivos salvos
//                2024-04-01 -
//---------------------------------------------------------------------

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include "fibonacci.hpp"
#include "ordenacao.hpp"
#include "arquivosAleatorios.hpp"

using namespace std;
using namespace std::chrono;

void criaVetoresIguaisParaOrdenacao (int qtddArquivos, int qtddNumeros);
float mediaVector (vector<int> V);
float desvioPadrao (vector<int> V, float media);

int main() {
    srand(time(0));

    //Inicializando variáveis
    int op;
    int qtddArquivos = 50;
    int qtddNumeros = 10000;

    cout << "Selecione a opcao:" << endl;
    cout << "(0) Gerar conjunto de numeros aleatorios" << endl
         << "(1) Ordenacao" << endl
         << "(2) Fibonacci" << endl;

    cin >> op;

    if (op == 0) criarArquivos(qtddArquivos, qtddNumeros);
    else if (op == 1) criaVetoresIguaisParaOrdenacao(qtddArquivos, qtddNumeros);
    else if (op == 2) SegundosFibonacci();
    else cout << "opcao inválida!" << endl;

    return 0;
}


void criaVetoresIguaisParaOrdenacao (int qtddArquivos, int qtddNumeros){
    string nomeArquivo;
    vector<int> dados;
    vector<int> vetorCopia;

    //Tempo marcado em milissegundos
    vector<int> tempoQRP; // QuickSort Recursivo Puro
    vector<int> tempoQRI; // QuickSort Recursivo Com Insercao
    vector<int> tempoQNRP; // QuickSort Não Recursivo Puro
    vector<int> tempoQNRI; // QuickSort Não Recursivo Com Insercao


    for (int i = 1; i <= qtddArquivos; i++){
        nomeArquivo = "arq/arquivo" + to_string(i) + ".txt";
        dados = leArquivo(nomeArquivo);
        
        // ---------- QuickSort Recursivo Puro ---------- 
        vetorCopia.assign(dados.begin(), dados.end());
        time_point start = high_resolution_clock::now();
        QuicksortRecursivoPuro(vetorCopia, 0, vetorCopia.size() - 1);
        time_point stop = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(stop - start);
        tempoQRP.push_back((int) duration.count());

        // ---------- QuickSort Recursivo Com Insercao ----------
        vetorCopia.assign(dados.begin(), dados.end());
        start = high_resolution_clock::now();
        QuicksortRecursivo_InsertionSort(vetorCopia, 0, vetorCopia.size()-1);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        tempoQRI.push_back((int) duration.count());

        // ---------- QuickSort Não Recursivo Puro ----------
        vetorCopia.assign(dados.begin(), dados.end());
        start = high_resolution_clock::now();
        QuicksortNaoRecursivoPuro(vetorCopia);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        tempoQNRP.push_back((int) duration.count());

        // ---------- QuickSort Não Recursivo Com Insercao ----------
        vetorCopia.assign(dados.begin(), dados.end());
        start = high_resolution_clock::now();
        QuicksortNaoRecursivo_InsertionSort(vetorCopia);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        tempoQNRI.push_back((int) duration.count());

    }

    float mediaQRP = mediaVector(tempoQRP);
    float mediaQRI = mediaVector(tempoQRI);
    float mediaQNRP = mediaVector(tempoQNRP);
    float mediaQNRI = mediaVector(tempoQNRI);

    cout << endl <<"Media de temp QRP = " << mediaQRP << endl;
    cout << "Media de temp QRI = " << mediaQRI << endl;
    cout << "Media de temp QNRP = " << mediaQNRP << endl;
    cout << "Media de temp QNRI = " << mediaQNRI << endl << endl;

    cout << "Desvio padrao de QRP = " << desvioPadrao(tempoQRP, mediaQRP) << endl;
    cout << "Desvio padrao de QRI = " << desvioPadrao(tempoQRI, mediaQRI) << endl;
    cout << "Desvio padrao de QNRP = " << desvioPadrao(tempoQNRP, mediaQNRP) << endl;
    cout << "Desvio padrao de QNRI = " << desvioPadrao(tempoQNRI, mediaQNRI) << endl;
}

float mediaVector (vector<int> V){
    float media = 0;
    for (int i = 0; i < (int)V.size(); i++){
        media += V[i];
    }
    media = (media / (int)V.size());
    return media;
}

float desvioPadrao (vector<int> V, float media){
    float desvioPadrao = 0;

    for(int i = 0; i < (int)V.size(); i++){
        desvioPadrao = desvioPadrao + pow((V[i] - media), 2);
    }
    return desvioPadrao;
} 
