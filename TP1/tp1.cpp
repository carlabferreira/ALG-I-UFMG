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
//---------------------------------------------------------------------


#include <iostream>
#include <vector>
#include <cmath> //para atan2
#include <algorithm> // Para a função std::find

using namespace std;

// DEFINICAO DAS FUNCOES UTILIZADAS
void LeGrafo(int N, int M, vector<vector<int>>& grafo, vector<pair<double, double>>& coordenadas, vector<vector<bool>>& visitado);
void SeparaPontes(int N, vector<vector<int>> grafo, vector<int>& pontosArticulacao, vector<int>& naoPontosArticulacao);
void EhPonte (int N, vector<vector<int>>& grafo, vector<bool>& v);
double InclinacaoRelativa(double x1, double y1, double x2, double y2); //Modificada da dada no enunciado
vector<int> OrdenacaoPorPosicoesRelativas (int i, vector<int>& vizinhos, vector<pair<double, double>> coordenadas);//organiza as conexões de cada vértice em ordem crescente de inclinação relativa em relação ao vértice de referência
void DFS(int limite, int i, int j, int verticeAtual, vector<vector<int>>& grafo, vector<vector<bool>>& visitado, vector<int>& face, vector<vector<int>>& faces);
void PrintaFaces(vector<vector<int>> faces);
void FaceUnica(int N, vector<vector<int>>& grafo, vector<pair<double, double>>& coordenadas, vector<vector<bool>>& visitado);

void dfs_arvore(int node, vector<vector<int>>& tree, vector<bool>& visited, vector<int>& path);
void printPath(const vector<int>& path);

void PrintaGrafo_Vizinhos (int N, vector<vector<int>>& grafo);
void PrintaGrafo_Coordenadas (int N, vector<pair<double, double>>& coordenadas);

int main() {
  // Ler o grafo da entrada
  int N, M;
  cin >> N >> M; //N vertices M arestas

  // DEFINIÇÃO DAS ESTRUTURAS PARA REPRESENTAR O GRAFO
  vector<vector<int>> grafo(N);
  vector<vector<bool>> visitado(N); // Semelhante a cor na implementação padrao de DFS
  vector<pair<double, double>> coordenadas(N); 
  
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
    // cout << 1 << endl << grafo[0].size()+1 << ' ';
    // for (int j = 0; j < grafo[0].size(); j++){
    //   cout << grafo[0][j] + 1 << ' ';
    // }
    // cout << 1 << endl;
    // return 0;
    FaceUnica(N, grafo, coordenadas, visitado);
    return 0;
  }

  // ------------- PARTE 2 - IDENTIFICA ARESTAS RELEVANTES / PONTES ------------- 
  // Ponte = uma aresta que, se removida, aumentará o número de componentes conectados do grafo, ou seja, 
  // a remoção da ponte desconectará alguma parte do grafo
  // Tem como objetivo identificar possiveis vertices 'isolados' e componentes conexos que se unem por uma aresta
  
  // Ponto de Articulação =  um vértice cuja remoção aumenta o número de componentes conectados do grafo. 
  // Em outras palavras, é um vértice que, se removido, desconecta partes do grafo.

  // DEFINIÇÃO DOS VETORES UTILIZADOS PARA DFS E IDENTIFICAÇÃO DAS FACES
  vector<int> pontosArticulacao, naoPontosArticulacao;
  if (M == (N-1)) {
    FaceUnica(N, grafo, coordenadas, visitado);
    return 0;
  }
  
  SeparaPontes(N, grafo, pontosArticulacao, naoPontosArticulacao);

  // Se todos os vertices são pontes então o grafo é uma arvore e so possui uma face
  // Se Arestas = Vertices -1 tbm é arvore
  // Se arvore, todas as arestas sao pontes (e todos os vertices internos(não folhas) sao pontos de articulaçao)
  if (pontosArticulacao.size() == N) {  
    FaceUnica(N, grafo, coordenadas, visitado);
    return 0;
  }
  

  // Se algum vertice faltando, erro ao identificar pontes
  if (pontosArticulacao.size() + naoPontosArticulacao.size() != N) return(-1);
  
  // cout << "Pontos de Articulação: ";
  // for (int i = 0; i< pontosArticulacao.size(); i++){
  //   cout << pontosArticulacao[i] << ' ';
  // }
  
  // cout << "Nao Pontos de Articulação: ";
  // for (int i = 0; i< naoPontosArticulacao.size(); i++){
  //   cout << naoPontosArticulacao[i] << ' ';
  // }
  
  // ordem para visitar = primeiro os "vertices comuns" e depois os pontos de articulçao
  vector<int> ordemVisita;
  for (int vertice : naoPontosArticulacao) ordemVisita.push_back(vertice);
  for (int vertice : pontosArticulacao) ordemVisita.push_back(vertice);
  //for (int i = 0; i < N; i++) ordemVisita.push_back(i);

  // for (int i = 0; i < N; i++) cout << ordemVisita[i] << ' ';
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
  
  // -------------
  // PrintaGrafo_Vizinhos(N, grafo);
  // PrintaGrafo_Coordenadas(N, coordenadas);
 
  return 0;
}

// Função para ler a entrada e construir o grafo
void LeGrafo(int N, int M, vector<vector<int>>& grafo, vector<pair<double, double>>& coordenadas, vector<vector<bool>>& visitado) {
  
  //definição das variaveis auxiliares
  double x, y;
  pair <double, double> aux;
  int grauVertice; //Quantidade de vizinhos
  int vizinho;

  for (int i = 0; i < N; ++i) { // Ler as coordenadas e graus dos nós
    cin >> x >> y;
    aux.first = x;
    aux.second = y;
    coordenadas[i] = aux;

    cin >> grauVertice;
    for (int j = 0; j < grauVertice; ++j) { // Ler os vizinhos dos nós
      cin >> vizinho;
      vizinho--; // Ajustar índices para 0-based
      grafo[i].push_back(vizinho);
      visitado[i].push_back(false);
    }
  }
}

void SeparaPontes(int N, vector<vector<int>> grafo, vector<int>& pontosArticulacao, vector<int>& naoPontosArticulacao){
  for (int i = 0; i < N; i++){
    bool terminar = false;
    for (int j = 0; j < grafo[i].size(); j++){
      //Criação de uma cópia do grafo e remoção de uma aresta:
      vector<bool> cor(N, false);
      vector<vector<int>> grafoAux = grafo; // Cópia do grafo original.
      int a = grafoAux[i][j]; // j-esimo vizinho do no i

      vector<int>::iterator it = find(grafoAux[i].begin(), grafoAux[i].end(), a);
      grafoAux[i].erase(it); // Remoção da aresta i-a do grafo copiado.
      it = find(grafoAux[a].begin(), grafoAux[a].end(), i);
      grafoAux[a].erase(it); // Remoção da aresta a-i do grafo copiado.

      int componentes = 0; //Contagem de componentes conectados
      for (int k = 0; k < N; k++)
        if (cor[k] == false){
          if (componentes == 1){
              terminar = true;
              break;
          }
          EhPonte(k, grafoAux, cor);
          componentes++;
        }
      if (terminar) break;
    }

    if (terminar) pontosArticulacao.push_back(i);
    else naoPontosArticulacao.push_back(i);
  }
}

void EhPonte (int k, vector<vector<int>>& grafo, vector<bool>& cor){
  cor[k] = true;  // Marca o nó atual como visitado
  // Itera sobre todos os vizinhos do nó atual
  for (auto vizinho : grafo[k]) {
    // Se o vizinho ainda não foi visitado, chama a função recursivamente
    if (cor[vizinho] == false) {
        EhPonte(vizinho, grafo, cor);
    }
  }
}

double InclinacaoRelativa(double x1, double y1, double x2, double y2){
  return atan2(y1 - y2, x1 - x2);
}

// Para cada vertice v, ordena seus adjacentes (na lista de adj) de acordo com a inclinação com v inicial
vector<int> OrdenacaoPorPosicoesRelativas (int i, vector<int>& vizinhos, vector<pair<double, double>> coordenadas){
  double x_ref = coordenadas[i].first;
  double y_ref = coordenadas[i].second;
  sort( vizinhos.begin(), vizinhos.end(), [&](double a, double b) {
        double incl_a = InclinacaoRelativa(coordenadas[a].first, coordenadas[a].second, x_ref, y_ref);
        double incl_b = InclinacaoRelativa(coordenadas[b].first, coordenadas[b].second, x_ref, y_ref);
        return incl_a < incl_b;
      }
  );

    return vizinhos;
}

// Função para ordenar os vizinhos usando Bubble Sort
/*vector<int> OrdenacaoPorPosicoesRelativas(int i, vector<int>& vizinhos, vector<pair<double, double>> coordenadas) {
    double x_ref = coordenadas[i].first;
    double y_ref = coordenadas[i].second;

    int n = vizinhos.size();
    for (int j = 0; j < n - 1; j++) {
        for (int k = 0; k < n - j - 1; k++) {
            double incl_a = InclinacaoRelativa(coordenadas[vizinhos[k]].first, coordenadas[vizinhos[k]].second, x_ref, y_ref);
            double incl_b = InclinacaoRelativa(coordenadas[vizinhos[k + 1]].first, coordenadas[vizinhos[k + 1]].second, x_ref, y_ref);
            if (incl_a > incl_b) {
                swap(vizinhos[k], vizinhos[k + 1]);
            }
        }
    }

    return vizinhos;
}*/

// Função para ordenar os vizinhos usando Insertion Sort
/*vector<int> OrdenacaoPorPosicoesRelativas(int i, vector<int>& vizinhos, vector<pair<double, double>> coordenadas) {
    double x_ref = coordenadas[i].first;
    double y_ref = coordenadas[i].second;

    int n = vizinhos.size();
    for (int j = 1; j < n; j++) {
        int key = vizinhos[j];
        double incl_key = InclinacaoRelativa(coordenadas[key].first, coordenadas[key].second, x_ref, y_ref);
        int k = j - 1;

        while (k >= 0) {
            double incl_k = InclinacaoRelativa(coordenadas[vizinhos[k]].first, coordenadas[vizinhos[k]].second, x_ref, y_ref);
            if (incl_key >= incl_k) {
                break;
            }
            vizinhos[k + 1] = vizinhos[k];
            k--;
        }
        vizinhos[k + 1] = key;
    }

    return vizinhos;
}*/

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

void FaceUnica(int N, vector<vector<int>>& grafo, vector<pair<double, double>>& coordenadas, vector<vector<bool>>& visitado){
  //cout << 1 << endl << N << ' ';
  
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


void PrintaGrafo_Vizinhos (int N, vector<vector<int>>& grafo){
  for (int i = 0; i < N; i++){
    cout << "Vertice " << i+1 << " tem como vizinhos ";
    for (int j = 0; j < grafo[i].size(); j++){
      cout << grafo[i][j] + 1 << ' ';
    }
    cout << endl;
  }
}

void PrintaGrafo_Coordenadas (int N, vector<pair<double, double>>& coordenadas){
  for (int i = 0; i < N; i++){
    cout << "Vertice " << i+1 << " tem como coordenadas " << coordenadas[i].first << ' ' << coordenadas[i].second << endl;
  }
}
