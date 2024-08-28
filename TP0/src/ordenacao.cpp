//---------------------------------------------------------------------
// Arquivo	: avaliaOrdenacao.cpp
// Conteudo	: implementacao da parte 1 do TP0
// Autor	: Carla Beatriz Ferreira (2022097470)
// Historico	: 2024-03-22 - arquivo criado
//                2024-
//---------------------------------------------------------------------

/*
    implementar o Quicksort tanto a versão recursiva quanto a não recursiva.
    Em ambos os casos, haverá duas possibilidade: a primeira é usar a respectiva versão do Quicksort para fazer a
    ordenação dos n elementos e a segunda é quando houver partições de no máximo 30 elementos, você deve usar a
    técnica de ordenação por inserção nessa partição.

    O que deve ser feito: Gerar aleatoriamente 50 conjuntos de 10000 números, aplicar as diferentes estratégias
    de ordenação e calcular os tempos de cada execução. Faça uma análise estatística dos resultados apresentando
    cada média com o respectivo desvio-padrão.

    Veja que serão quatro resultados:
        (i) Quicksort recursivo 'puro'
        (ii) Quicksort recursivo & Ordenação por inserção
        (iii) Quicksort não recursivo 'puro'
        (iv) Quicksort não recursivo & Ordenação por inserção
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <stack>
#include <utility>
#include "ordenacao.hpp"

using namespace std;

void InsertionSort(vector<int>& V, int esq, int dir){
    for (int i = esq + 1; i <= dir ; i++) { 
		
		int j = i;
	
		while (j > esq && V[j] < V[j-1]) {
			int aux = V[j];
			V[j] = V[j - 1];
			V[j - 1] = aux;
			j -= 1;
		}
	}
}


// ---------------------------- QuicksortRecursivoPuro ------------------------------------------


void QuicksortRecursivoPuro (vector<int> &V, int esq, int dir) {
    if(esq < dir) {
        int indicePivo = particao(V, esq, dir);
        QuicksortRecursivoPuro(V, esq, indicePivo - 1);
        QuicksortRecursivoPuro(V, indicePivo, dir);
    }
}


int particao(vector<int>& V, int esq, int dir) {
    Mediana_3valores(V, dir, esq); //coloca a mediana no ultimo elemento
    int pivot = V[dir]; //pivo é o ultim elemento
    int i = esq - 1; // índice do menor elemento

    for (int j = esq; j <= dir - 1; j++) {
        // se o elemento atual for menor ou igual ao pivô
        if (V[j] <= pivot) {
            i++; // incrementa o índice do menor elemento
            Troca(V[i], V[j]); // troca V[i] e V[j]
        }
    }
    Troca(V[i + 1], V[dir]); // troca V[i + 1] e V[dir] (pivô)
    return (i + 1);
}



// ---------------------------- QuicksortRecursivo_InsertionSort --------------------------------
void QuicksortRecursivo_InsertionSort(vector<int> &V, int esq, int dir){
    if(esq < dir) {
        if ((dir - esq) <= 30) InsertionSort(V, esq, dir); 
        else {
            int indicePivo = particao(V, esq, dir);
            QuicksortRecursivo_InsertionSort(V, esq, indicePivo - 1);
            QuicksortRecursivo_InsertionSort(V, indicePivo, dir);
        }
    }
}

// ---------------------------- QuicksortNaoRecursivoPuro ------------------------------------------
void QuicksortNaoRecursivoPuro (vector<int>& V) {
    // Cria uma pilha para armazenar os limites do subvetor
    stack<pair<int, int>> pilha;
    pilha.push(make_pair(0, V.size()-1)); // empilha os limites iniciais

    // Enquanto a pilha não estiver vazia
    while (!pilha.empty()) {
        // Desempilha os limites do subvetor atual
        int inicio = pilha.top().first;
        int fim = pilha.top().second;
        pilha.pop();

        // Particiona o subvetor e obtém a posição final do pivô
        int pivoFinal = particao(V, inicio, fim);

        // Se houver elementos à esquerda do pivô, empilhe seus limites
        if (pivoFinal - 1 > inicio)
            pilha.push(make_pair(inicio, pivoFinal - 1));
        
        // Se houver elementos à direita do pivô, empilhe seus limites
        if (pivoFinal + 1 < fim)
            pilha.push(make_pair(pivoFinal + 1, fim));
    }
}

// ---------------------------- QuicksortNaoRecursivo_InsertionSort ---------------------------------
void QuicksortNaoRecursivo_InsertionSort (vector<int>& V) {
    if(V.size() <= 30) {
        InsertionSort(V, 0, (V.size()-1));
    }
    // Cria uma pilha para armazenar os limites do subvetor
    stack<pair<int, int>> pilha;
    pilha.push(make_pair(0, V.size()-1)); // empilha os limites iniciais


    // Enquanto a pilha não estiver vazia
    while (!pilha.empty()) {
        // Desempilha os limites do subvetor atual
        int inicio = pilha.top().first;
        int fim = pilha.top().second;
        pilha.pop();
        if ((fim - inicio) <= 30) InsertionSort(V, inicio, fim);
        else {
            // Particiona o subvetor e obtém a posição final do pivô
            int pivoFinal = particao(V, inicio, fim);

            // Se houver elementos à esquerda do pivô, empilhe seus limites
            if (pivoFinal - 1 > inicio) pilha.push(make_pair(inicio, pivoFinal - 1));
            
            // Se houver elementos à direita do pivô, empilhe seus limites
            if (pivoFinal + 1 < fim) pilha.push(make_pair(pivoFinal + 1, fim));
        }
    }
}

void Troca(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void Mediana_3valores (vector<int> &V, int dir, int esq){ //coloca o valor mediano na ultima posicao
    //indices aleatorios a, b e c
    int n = (dir - esq) + 1;
    int a = esq + rand()%n;
    int b = esq + rand()%n;
    int c = esq + rand()%n;

    while ((a == b || a == c || b == c) && (n >= 3)) { // Verifica se os índices são iguais
        if (a == b) a = esq + rand() % n;
        if (a == c) a = esq + rand() % n;
        if (b == c) b = esq + rand() % n;
    }

    if ((V[a] <= V[b] && V[a] >= V[c]) || (V[a] >= V[b] && V[a] <= V[c])){
        Troca(V[dir], V[a]);
    }
    else if ((V[b] <= V[a] && V[b] >= V[c]) || (V[b] >= V[a] && V[b] <= V[c])){
        Troca(V[dir], V[b]);;
    }
    else{
        Troca(V[dir], V[c]);
    }   
}