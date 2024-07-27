//---------------------------------------------------------------------
// Arquivo	: tp1.cpp
// Conteudo	: implementacao do TP1
// Autor	: Carla Beatriz Ferreira (2022097470)
// Historico:   2023-05-11 - arquivo criado
//              2024-05-12 - estruturação da estrutura dos vertices - vilas
//              2024-06-30 - mudança do tipo de representacao de estrutura do grafo 
//              2024-07-07 - mudança para grafo com lista de adjacencia e vector<vector<>> assim como tp1
//              2024-07-10 - Implementação de LeGrafo, baseada no tp1.cpp e mudanças de acordo com enunciado do tp2
//---------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <set>
#include <cmath>   
using namespace std;

// DEFINICAO DAS ESTRUTURAS DE DADOS UTILIZADAS
typedef struct CaminhoDeVilasFormadoPorArestas {
    long long int outroNoDaAresta;
    long long int anoFinalConstrucao;
    long long int tempoTravessia;
    long long int custoConstrucao;
} vila;

// DEFINICAO DAS FUNCOES UTILIZADAS
void LeGrafo(int N, int M, vector<vector<vila>> &grafo);


int main (){
    long long int N, M;
    cin >> N >> M; // N vertices M arestas
    // DEFINIÇÃO DAS ESTRUTURAS PARA REPRESENTAR O GRAFO
    vector<vector<vila>> grafo(N);


    // ------------- PARTE 1 - LEITURA DO GRAFO  -------------
    LeGrafo(N, M, grafo);

    if (grafo.size() != N){
        cout << "Erro na leitura do grafo";
        cout << "grafo.size() = " << grafo.size() << endl;
        return (-1);
    }

    // É garantido que o grafo é conexo, N >= 1
    // mas não é garantido que não é uma arvore/grafo sem ciclos
    if (M == N-1){ // quantidade de arestas é quantidade de vertices -1 = arvore  //todo
        // todo
        return 0;
    }

    // ------------- PARTE 2 - DFS PARA ACHAR ANOS A1 E A2  -------------
    // Do enunciad0: A1 (...) representa o primeiro ano no qual as distâncias listadas nas linha anteriores são mutuamente realizáveis
    //               A2 (...) representa o primeiro ano a partir do qual é possível chegar em qualquer vila do reino a partir do palácio real.
    
    //todo


    // ------------- PARTE 3 - DIJKSTRA PARA ACHAR DISTANCIA MINIMA DO PALACIO PARA OUTRA VILA -------------
  
    //todo
    

    // ------------- PARTE 4 - ARVORE MINIMA DE CUSTOS - PRIM
  
    //todo

    
    return 0;
}

void LeGrafo(int N, int M, vector<vector<vila>> &grafo){
    long long int u, v, a, l, c;
    vila v1, v2; // Para criar as arestas nas duas direções no grafo
    set<long long int> conjuntoDeAnos; // set e nao vector pois conjuntos não tem elementos repetidos
    vector<long long int> anos;
    for (int i =0; i < M; i++){
        cin >> u >> v >> a >> l >> c;
        u--; //0-based
        v--; //0-based
        
        v1.anoFinalConstrucao = a;
        v1.tempoTravessia = l;
        v1.custoConstrucao = c;
        v1.outroNoDaAresta = v;
        
        v2.anoFinalConstrucao = a;
        v2.tempoTravessia = l;
        v2.custoConstrucao = c;
        v2.outroNoDaAresta = u;

        grafo[u].push_back(v1);
        grafo[v].push_back(v2);
        conjuntoDeAnos.insert(a);
    }

    // Mudança de anos em um conjunto para um vetor
    for (auto it = conjuntoDeAnos.begin(); it != conjuntoDeAnos.end(); ++it) {
        anos.push_back(*it);
    }

    //?
    //cout << "grafo[4][0].outroNO = "<< grafo[3][0].outroNoDaAresta+1 <<  " e grafo[4][0].custo = " << grafo[3][0].custoConstrucao << endl;
    //cout << "grafo[4][1].outroNO = "<< grafo[3][1].outroNoDaAresta+1 <<  " e grafo[4][1].custo = " << grafo[3][1].custoConstrucao << endl;
}
