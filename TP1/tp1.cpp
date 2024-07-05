#include <iostream>
#include <vector>

using namespace std;


// Função para ler a entrada e construir o grafo
void readGraph(int& N, int& M, vector<vector<int>> graph) { //Todo
  // Inicializar matriz de adjacência com zeros
  /*for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      adj[i][j] = 0;
    }
  }
  
  for (int i = 0; i < N; ++i) { // Ler as coordenadas e graus dos nós
    cin >> graph[i].x >> graph[i].y >> graph[i].degree;
    for (int j = 0; j < graph[i].degree; ++j) { // Ler os vizinhos dos nós
      int neighbor;
      cin >> neighbor;
      graph[i].neighbors.push_back(neighbor - 1); // Ajustar índices para 0-based

      // Marcando arestas na matriz de adjacência
      adj[i][neighbor - 1] = 1;
      adj[neighbor - 1][i] = 1; // Marcação para grafos não direcionados
    }
    graph[i].cor = 0; // define a cor de todos como branca
  }
  */
}

// Função para imprimir o grafo (opcional)
/*
void printGraph(const vector<Node>& graph) {
  for (int i = 0; i < graph.size(); ++i) {
    cout << "Nó " << i + 1 << ": (coordenadas = (" << graph[i].x << ", " << graph[i].y << "), grau = " << graph[i].degree << ")" << endl;
    cout << "Vizinhos: ";
    for (int neighbor : graph[i].neighbors) {
      cout << neighbor + 1 << " ";
    }
    cout << endl;
  }
}*/


int main() {
  // Ler o grafo da entrada
  int N, M;
  cin >> N >> M; //N vertices M arestas

  // DEFINIÇÃO DAS ESTRUTURAS PARA REPRESENTAR O GRAFO
  vector<vector<int>> graph(N);
  vector<vector<bool>> visited(N);
  vector<pair<double, double>> positions(N); 

  readGraph(N, M, graph);
  //printGraph(adj); // Chamada opcional para imprimir o grafo

  return 0;
}
