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
#include <queue>
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
void LeGrafo(long long int N, long long int M, vector<vector<vila>> &grafo, vector<long long int> &anos);
void MenorAnoConectavel_BFS(vector<vector<vila>> &grafo, long long int N, long long int &a2, vector<long long int> anos);
void BFS(vector<vector<vila>> &grafo, vector<bool> &visitado, long long int inicio, long long int final);

int main (){
    // DEFINIÇÃO DAS VARIAVEIS
    long long int N, M;
    cin >> N >> M; // N vertices M arestas

    vector<vector<vila>> grafo(N);
    vector<long long int> anos;
    
    
    // ------------- PARTE 1 - LEITURA DO GRAFO  -------------
    LeGrafo(N, M, grafo, anos);

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
    // Do enunciad0: A1 (...) representa o primeiro ano no qual as distâncias listadas nas linha anteriores são mutuamente realizáveis (passado em dijkstra)
    //               A2 (...) representa o primeiro ano a partir do qual é possível chegar em qualquer vila do reino a partir do palácio real.
    
    long long int a1 = 0, a2 = 0;

    // Realiza uma busca binária para encontrar o menor ano a partir do qual o grafo se torna conectado
    // Modifica a2 passada por referência
    MenorAnoConectavel_BFS(grafo, N, a2, anos);

    // ------------- PARTE 3 - DIJKSTRA PARA ACHAR DISTANCIA MINIMA DO PALACIO PARA OUTRA VILA -------------
  
    //todo
    

    // ------------- PARTE 4 - ARVORE MINIMA DE CUSTOS - PRIM
  
    //todo

    
    return 0;
}

void LeGrafo(long long int N, long long int M, vector<vector<vila>> &grafo, vector<long long int> &anos){
    long long int u, v, a, l, c;
    vila v1, v2; // Para criar as arestas nas duas direções no grafo
    set<long long int> conjuntoDeAnos; // set e nao vector pois conjuntos não tem elementos repetidos
    
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
    // cout << "grafo[4][0].outroNO = "<< grafo[3][0].outroNoDaAresta+1 <<  " e grafo[4][0].custo = " << grafo[3][0].custoConstrucao << endl;
    // cout << "grafo[4][1].outroNO = "<< grafo[3][1].outroNoDaAresta+1 <<  " e grafo[4][1].custo = " << grafo[3][1].custoConstrucao << endl;
}

void MenorAnoConectavel_BFS(vector<vector<vila>> &grafo, long long int N, long long int &a2, vector<long long int> anos){
    long long int esq = 0;
    long long int dir = anos.size() - 1;

    while (esq <= dir){
        long long int count = 0;
        long long int meio = esq + (dir - esq)/2;
        long long int anoAtual = anos[meio];
        vector<bool> visitado(N, false);

        for(long long int j = 0; j < N; j++) { // Itera sobre todos os nós do grafo
        // Se o nó não foi visitado, inicia uma DFS a partir dele
            if (!visitado[j]) {
                if (count == 2) break;
                
                // Executa DFS no grafo, marcando componentes conexas
                BFS(grafo, visitado, j, anoAtual);
                count++;
            }
        }

        // Verifica se o grafo é uma única componente conexa
        if (count == 1) {
            a2 = anoAtual;  // Atualiza n2 com o ano em que o grafo é conectado
            dir = meio - 1;  // Continua a busca binária na metade inferior
        } else {
            esq = meio + 1;  // Continua a busca binária na metade superior
        }
    }
}

void BFS(vector<vector<vila>>& graph, vector<bool>& visitado, long long int inicio, long long int final) {
    queue<long long int> fila;
    fila.push(inicio);
    visitado[inicio] = true;

    while (!fila.empty()) {
        long long int atual = fila.front();
        fila.pop();

        for (const vila& noVizinho : graph[atual]) {
            // Para cada uma das vilas conectados a aquele vertice atual
            // Proucura por arestas que não foram percorridas && construidas até aquele ano
            if (!visitado[noVizinho.outroNoDaAresta] && noVizinho.anoFinalConstrucao <= final) {
                visitado[noVizinho.outroNoDaAresta] = true;
                fila.push(noVizinho.outroNoDaAresta);
            }
        }
    }
}
