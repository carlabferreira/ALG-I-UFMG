//---------------------------------------------------------------------
// Arquivo	: fibonacci.cpp
// Conteudo	: implementacao da parte 2 do TP0
// Autor	: Carla Beatriz Ferreira (2022097470)
// Historico	: 2024-03-22 - arquivo criado
//                2024-
//---------------------------------------------------------------------


/*
    O objetivo é gerar uma tabela informando quantos números da sequência foram gerados usando tanto a versão
    recursiva quanto a iterativa considerando um tempo de processamento limitado a 15, 30, 45, 60, 75, 90, 105 e 120
    segundos.
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include "fibonacci.hpp"

using namespace std;

int fibonacciIterativo(int tempoLimite) {
    unsigned long long int primeiro = 0, segundo = 1, proximo;
    int n = 0;

    // Obtém o tempo inicial
    auto inicio = chrono::steady_clock::now();
    
    //vector<unsigned long long int> V;

    // Executa o cálculo da sequência de Fibonacci enquanto o tempo não atingir o limite
    while (true) {
        auto atual = chrono::steady_clock::now();
        auto duracao = chrono::duration_cast<chrono::seconds>(atual - inicio).count();

        if (duracao >= tempoLimite)
            break;

        if (n <= 1)
            proximo = n;
        else {
            proximo = primeiro + segundo;
            primeiro = segundo;
            segundo = proximo;
        }
        //V.push_back(proximo);
        n++;
    }

    // for (int i = 0; i < (int)V.size(); i++){
    //     cout << V[i] << endl;
    // }
    // cout << endl;
    return n;
}

// Função para calcular o n-ésimo termo da sequência de Fibonacci de forma recursiva
int fibonacciRecursivo(int n) {
    if (n <= 1)
        return n;
    return fibonacciRecursivo(n - 1) + fibonacciRecursivo(n - 2);
}

void SegundosFibonacci(void) {
    cout << "Digite a quantidade de segundos a ser executado: " << endl;
    int tempoLimite;
    cin >> tempoLimite;

    int resultado;
    //? OK
    resultado = fibonacciIterativo(tempoLimite);
    cout << "Número máximo de Fibonacci Iterativo calculado em " << tempoLimite << " segundos: " << resultado << endl;

    ContaTempoRecursivo(tempoLimite); //? OK

}

void ContaTempoRecursivo (int tempoLimite) {
    //? funciona esquisito tbm, reinicia o tempo e faz dnv ate aquele outro - pode ser considerado correto de acordo com a c=documentação
    
    //vector<int> V;
    // Inicia uma thread com a função fibonacciRecursivo
    std::thread t([&tempoLimite](){
        int n = 0;
        auto inicio = chrono::steady_clock::now();
        bool tempoEsgotado = false;

        // Calcula os termos da sequência de Fibonacci até que o tempo limite seja atingido
        while (!tempoEsgotado) {
            auto atual = chrono::steady_clock::now();
            auto duracao = chrono::duration_cast<chrono::seconds>(atual - inicio).count();

            if (duracao >= tempoLimite)
                tempoEsgotado = true;
            fibonacciRecursivo(n);
            //cout << fibonacciRecursivo(n) << " "; // Exibe o resultado
            //cout.flush(); // Limpa o buffer de saída
            n++;
        }
        cout << "Número máximo de Fibonacci Recursivo calculado em " << tempoLimite << " segundos: " << n << endl;

    });
    
    // Aguarda até que a thread termine ou até que o tempo limite seja atingido
    t.join();
}
