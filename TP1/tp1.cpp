//---------------------------------------------------------------------
// Arquivo	: tp1.cpp
// Conteudo	: implementacao do TP1
// Autor	: Carla Beatriz Ferreira (2022097470)
// Historico	: 2023-04-01 - arquivo criado
//              2024-04-15 - organização das bibliotecas "montaGrafo.hpp" e "descobreFaces"
//              2024-04-27 - organização da classe grafo utilizando struct Node e formato de lista de adjacencia com pointer
//              2024-05-15 - edicoes na logica do codigo, LeGrafo e BFS
//              2024-06-13 - mudança de lista de adjacencia para matriz de adjacencia
//              2024-06-25 - mudança (volta) de matriz para lista de adjacencia, sem estrutura node com ponteiros
//              2024-07-01 - junção dos arquivos .hpp e .cpp auxiliares em um só - tp1.cpp devido ao formato de entrega no moodle
//              2024-07-06 - edicoes em LeGrafo e adições de tratamento de excecoes com return (-1) na main
//              2024-07-07 - adicao da ordem de visita e testes com exemplos do enunciado
//              2024-07-07 - edicao na estrutura e funcionamento de DFS
//              2024-07-07 - adicao da funcao dfs para arvores, tratamento de caso
//---------------------------------------------------------------------


#include <iostream>
#include <vector>
#include <cmath> //para atan2
#include <algorithm> // Para a função std::find

using namespace std;

// DEFINICAO DAS FUNCOES UTILIZADAS
void LeGrafo(int N, int M, vector<vector<int>>& grafo, vector<pair<long double, long double>>& coordenadas, vector<vector<bool>>& visitado);
long double InclinacaoRelativa(long double x1, long double y1, long double x2, long double y2); //Modificada da dada no enunciado
vector<int> OrdenacaoPorPosicoesRelativas (int i, vector<int>& vizinhos, vector<pair<long double, long double>> coordenadas);//organiza as conexões de cada vértice em ordem crescente de inclinação relativa em relação ao vértice de referência
void DFS(int limite, int i, int j, int verticeAtual, vector<vector<int>>& grafo, vector<vector<bool>>& visitado, vector<int>& face, vector<vector<int>>& faces);
void PrintaFaces(vector<vector<int>> faces);
void FaceUnica(int N, vector<vector<int>>& grafo, vector<pair<long double, long double>>& coordenadas, vector<vector<bool>>& visitado);

void dfs_arvore(int node, vector<vector<int>>& tree, vector<bool>& visited, vector<int>& path);
void printPath(const vector<int>& path);
void DFS_new(int u, int parent, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& discovery, vector<int>& low, vector<bool>& articulationPoint, int& time);
void findArticulationPoints(int n, vector<vector<int>>& adj, vector<int>& pontosArticulacao, vector<int>& naoPontosArticulacao);

int main() {
  // Ler o grafo da entrada
  int N, M;
  cin >> N >> M; //N vertices M arestas

  // DEFINIÇÃO DAS ESTRUTURAS PARA REPRESENTAR O GRAFO
  vector<vector<int>> grafo(N);
  vector<vector<bool>> visitado(N); // Semelhante a cor na implementação padrao de DFS
  vector<pair<long double, long double>> coordenadas(N); 
  
  // ------------- PARTE 1 - LEITURA DO GRAFO  ------------- 
  LeGrafo(N, M, grafo, coordenadas, visitado);

  if (grafo.size() != N) {
    cout << "Erro na leitura do grafo";
    cout << "grafo.size() = " << grafo.size() << endl;
    return (-1);
  }

  // É garantido que o grafo é conexo, N >= 1 e que um vertice não é vizinho de si mesmo, 
  // mas não é garantido que não é uma arvore/grafosem ciclos
  if (N <= 2){ // Se 2 ou menos vertices então só tem uma face
    FaceUnica(N, grafo, coordenadas, visitado);
    return 0;
  }
  
  // ------------- PARTE 2 - IDENTIFICA ARESTAS RELEVANTES / PONTES ------------- 
  // DEFINIÇÃO DOS VETORES UTILIZADOS PARA DFS E IDENTIFICAÇÃO DAS FACES
  vector<int> pontosArticulacao, naoPontosArticulacao;
  if (M == (N-1)) { // Se Arestas = Vertices -1 é arvore
    FaceUnica(N, grafo, coordenadas, visitado);
    return 0;
  }
  
  findArticulationPoints(N, grafo, pontosArticulacao, naoPontosArticulacao);

  // Se algum vertice faltando, erro ao identificar pontes
  if (pontosArticulacao.size() + naoPontosArticulacao.size() != N) return(-1);

  // ordem para visitar = primeiro os "vertices comuns" e depois os pontos de articulçao
  vector<int> ordemVisita;
  for (int vertice : naoPontosArticulacao) ordemVisita.push_back(vertice);
  for (int vertice : pontosArticulacao) ordemVisita.push_back(vertice);

  // Para cada vertice v, ordena seus adjacentes (na lista de adj) de acordo com a inclinação com v inicial
  for(int i = 0; i < N; i++) grafo[i] = OrdenacaoPorPosicoesRelativas(i, grafo[i], coordenadas);
  
  // ------------- PARTE 3 - VISTITA DOS VERTICES E UTILIZACAO DE DFS PARA IDENTIFICAR FACES -------------
  vector<int> face;
  vector<vector<int>> faces;

  for(int i = 0; i < visitado.size(); i++){
    for(int j = 0; j < visitado[ordemVisita[i]].size(); j++){
      if(!visitado[ordemVisita[i]][j]) {
        face.push_back(ordemVisita[i]);
        DFS(ordemVisita[i], ordemVisita[i], j, grafo[ordemVisita[i]][j], grafo, visitado, face, faces);
      }
    }
  }

  // ------------- PARTE 4 - FINALIZAÇÃO E EXIBIÇÃO DAS FACES
  PrintaFaces(faces);
 
  return 0;
}

// Função para ler a entrada e construir o grafo
void LeGrafo(int N, int M, vector<vector<int>>& grafo, vector<pair<long double, long double>>& coordenadas, vector<vector<bool>>& visitado) {
  
  // Definição das variáveis auxiliares
  long double x, y;
  pair<long double, long double> aux;
  int grauVertice; // Quantidade de vizinhos
  int vizinho;

  for (int i = 0; i < N; ++i) { // Ler as coordenadas e graus dos nós
    cin >> x >> y;
    aux.first = x;
    aux.second = y;
    coordenadas[i] = aux;

    cin >> grauVertice;
    visitado[i].resize(grauVertice, false); // Inicializar todos os valores como false
    for (int j = 0; j < grauVertice; ++j) { // Ler os vizinhos dos nós
      cin >> vizinho;
      vizinho--; // Ajustar índices para 0-based
      grafo[i].push_back(vizinho);
      visitado[i][j] = false;
    }
  }
}

void DFS_new(int u, int parent, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& discovery, vector<int>& low, vector<bool>& articulationPoint, int& time) {
    visited[u] = true;
    discovery[u] = low[u] = ++time;
    int children = 0;

    for (int v : adj[u]) {
        if (!visited[v]) {
            children++;
            DFS_new(v, u, adj, visited, discovery, low, articulationPoint, time);

            // Verifica se a subárvore enraizada em v tem uma conexão de volta para um ancestral de u
            low[u] = min(low[u], low[v]);

            // (1) u é raiz da DFS e tem pelo menos dois filhos
            if (parent == -1 && children > 1)
                articulationPoint[u] = true;

            // (2) u não é raiz da DFS e low[v] >= discovery[u]
            if (parent != -1 && low[v] >= discovery[u])
                articulationPoint[u] = true;
        }
        else if (v != parent) {
            // Atualiza low[u] para a menor descoberta
            low[u] = min(low[u], discovery[v]);
        }
    }
}

void findArticulationPoints(int n, vector<vector<int>>& adj, vector<int>& pontosArticulacao, vector<int>& naoPontosArticulacao) {
    vector<bool> visited(n, false);
    vector<int> discovery(n, -1);
    vector<int> low(n, -1);
    vector<bool> articulationPoint(n, false);
    int time = 0;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            DFS_new(i, -1, adj, visited, discovery, low, articulationPoint, time);
        }
    }

    for (int i = 0; i < n; i++) {
        if (articulationPoint[i]) pontosArticulacao.push_back(i);
        else naoPontosArticulacao.push_back(i);
    }
}

long double InclinacaoRelativa(long double x1, long double y1, long double x2, long double y2){
  return atan2(y1 - y2, x1 - x2);
}

// Para cada vertice v, ordena seus adjacentes (na lista de adj) de acordo com a inclinação com v inicial
vector<int> OrdenacaoPorPosicoesRelativas (int i, vector<int>& vizinhos, vector<pair<long double, long double>> coordenadas){
  long double x_ref = coordenadas[i].first;
  long double y_ref = coordenadas[i].second;
  sort( vizinhos.begin(), vizinhos.end(), [&](long double a, long double b) {
        long double incl_a = InclinacaoRelativa(coordenadas[a].first, coordenadas[a].second, x_ref, y_ref);
        long double incl_b = InclinacaoRelativa(coordenadas[b].first, coordenadas[b].second, x_ref, y_ref);
        return incl_a < incl_b;
      }
  );

    return vizinhos;
}

void DFS(int limite, int i, int j, int verticeAtual, vector<vector<int>>& grafo, vector<vector<bool>>& visitado, vector<int>& face, vector<vector<int>>& faces){
  visitado[i][j] = true;
  //cout << "em dfs" << endl;
  if (limite == verticeAtual) {
      face.push_back(limite);
      faces.push_back(face); // termina aquela face e coloca em faceS
      face.clear(); // zera o vetor de face par ir para a proxima face
      return; 
  }

  int prox = 0;
  // Encontra o próximo índice (prox) a ser visitado.
  // Percorre a lista de adjacências do vértice verticeAtual até encontrar a próxima conexão não visitada.
  while (prox < grafo[verticeAtual].size()) {
    if (grafo[verticeAtual][prox] == i) {
      prox++; 
      break;
    }
    prox++;
  }

  // continua a buscar o próximo vértice k não visitado na lista de adjacências do verticeAtual
  for(; prox <= grafo[verticeAtual].size(); prox++){
      if(prox == grafo[verticeAtual].size()) prox = 0;
      if(visitado[verticeAtual][prox] == false) break;
  }

  face.push_back(verticeAtual);
  DFS(limite, verticeAtual, prox, grafo[verticeAtual][prox], grafo, visitado, face, faces);

}

void PrintaFaces(vector<vector<int>> faces){
    cout << faces.size() << endl;
    for(int i = 0; i < faces.size(); i++){
        cout << faces[i].size() << " ";
        for(int j = 0;  j < faces[i].size(); j++) 
          cout << faces[i][j]+1 << " "; // +1 pois é construido baseado em começar do zero
        cout << endl;
    }
}

void FaceUnica(int N, vector<vector<int>>& grafo, vector<pair<long double, long double>>& coordenadas, vector<vector<bool>>& visitado){
  //cout << "face unica" << endl;
  
  vector<int> ordemVisita;
  for (int i = 0; i < grafo[0].size(); i++) ordemVisita.push_back(i);

  for(int i = 0; i < N; i++) grafo[i] = OrdenacaoPorPosicoesRelativas(i, grafo[i], coordenadas);

  // Vetor para marcar nós visitados durante o DFS
  vector<bool> visited(N, false);
  // Vetor para armazenar o caminho encontrado durante o DFS
  vector<int> path;

  // Chamando DFS a partir do nó raiz 0 (pode ser qualquer nó na árvore)
  dfs_arvore(0, grafo, visited, path);
  printPath(path);
}

// Função DFS recursiva para percorrer a árvore e obter o caminho
void dfs_arvore(int node, vector<vector<int>>& tree, vector<bool>& visited, vector<int>& path) {
    visited[node] = true;
    path.push_back(node);

    // Verifica se o nó atual é uma folha (sem vizinhos além do pai)
    if (tree[node].size() == 1 && visited[tree[node][0]]) {
        // Adiciona o pai novamente ao caminho
        path.push_back(tree[node][0]);
    }

    for (int neighbor : tree[node]) {
        if (!visited[neighbor]) {
            dfs_arvore(neighbor, tree, visited, path);
            // Após retornar da recursão, se o nó vizinho for uma folha, adiciona o pai novamente
            if (path.back() == neighbor) {
                path.push_back(node);
            }
        }
    }
}

// Função para imprimir o caminho encontrado
void printPath(const vector<int>& path) {
  cout << 1 << endl << path.size() << ' ';
    for (int node : path) {
        cout << node+1 << " ";
    }
    cout << endl;
}
