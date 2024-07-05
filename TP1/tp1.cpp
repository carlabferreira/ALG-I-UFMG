#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
//----------------------- ANOTACOES ------------
// Dado que cada Aresta tem 2 faces,  uma estratégia tratada foi implementar uma
// estrutura Aresta com cor e visitá-la também, entretanto,  mudando a estrutura de lista encadeada 
// para matriz de adjacências corrige este problema de ter as arestas em 2 sentidos


// Estrutura para representar um nó
struct Node {
  int id; // Identificador do nó
  double x, y; // Coordenadas do nó no plano cartesiano
  int degree; // Grau do nó = numero de arestas que tem
  vector<int> neighbors; // Lista de vizinhos
  int cor; // 0 = branco, 1 = cinza, 2 = preto
};

struct Edge { // criada no sentido v1 para v2 //todo
    //vai ter uma "refletida" de 'v2' para 'v1' pela forma de construção, com a face a ser diferente
    int v2; 
    int cor;
    int face1;
}; //todo  Aresta[v1].v2 = vizinho de v1

// Função para ler a entrada e construir o grafo
vector<Node> readGraph(int& N, int& M) {
  vector<Node> graph(N);  // Criar vetor de nós
  vector<vector<Edge>> edges(N,M); //Aresta sentido v1->v2 e v2->v1
  for (int i = 0; i < N; ++i) {  // Ler as coordenadas e graus dos nós
    cin >> graph[i].x >> graph[i].y >> graph[i].degree;
    for (int j = 0; j < graph[i].degree; ++j) { // Ler os vizinhos dos nós
      int neighbor;
      cin >> neighbor;
      graph[i].neighbors.push_back(neighbor - 1); // Ajustar índices para 0-based
      edge[i][j].v2.push_back(
    }
    graph[i].cor = 0; // define a cor de todos como branca
  }
  return graph;
}



// Função para imprimir o grafo (opcional)
void printGraph(const vector<Node>& graph) {
  for (int i = 0; i < graph.size(); ++i) {
    cout << "Nó " << i + 1 << ": (coordenadas = (" << graph[i].x << ", " << graph[i].y << "), grau = " << graph[i].degree << ")" << endl;
    cout << "Vizinhos: ";
    for (int neighbor : graph[i].neighbors) {
      cout << neighbor + 1 << " ";
    }
    cout << endl;
  }
}

int main() {
  // Ler o grafo da entrada
  int N, M; //N vertices e M arestas
  cin >> N >> M;
  vector<Node> graph = readGraph(N, M);
  printGraph(graph);
  // Calcular e exibir a distância euclidiana máxima entre vizinhos de cada nó
  

  return 0;
}
 
    