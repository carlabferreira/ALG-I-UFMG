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
double InclinacaoRelativa(double x1, double x2, double y1, double y2); //Modificada da dada no enunciado
vector<int> PosicoesRelativasOrdenadas (int i, vector<int>& connections, vector<pair<double, double>> pos);//organiza as conexões de cada vértice em ordem crescente de inclinação relativa em relação ao vértice de referência


void PrintaGrafo_Vizinhos (int N, vector<vector<int>>& grafo);
void PrintaGrafo_Coordenadas (int N, vector<pair<double, double>>& coordenadas);

int main() {
  // Ler o grafo da entrada
  int N, M;
  cin >> N >> M; //N vertices M arestas

  // DEFINIÇÃO DAS ESTRUTURAS PARA REPRESENTAR O GRAFO
  vector<vector<int>> grafo(N);
  vector<vector<bool>> visitado(N);
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
    cout << 1 << ' ' << 1 << ' ';
    for (int j = 0; j < grafo[0].size(); j++){
      cout << grafo[0][j] + 1 << ' ';
    }
    cout << 1 << endl;
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
  SeparaPontes(N, grafo, pontosArticulacao, naoPontosArticulacao);

  //Se todos os vertices são pontes então o grafo é uma arvore e so possui uma face
  if (pontosArticulacao.size() == N) {  
    //todo
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

  // for (int i = 0; i < N; i++) cout << ordemVisita[i] << ' ';


  // ------------- PARTE 3 - VISTITA DOS VERTICES E UTILIZACAO DE DFS PARA IDENTIFICAR FACES -------------




  // ------------- PARTE 4 - FINALIZAÇÃO E EXIBIÇÃO DAS FACES
  
  
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

double InclinacaoRelativa(double x1, double x2, double y1, double y2){
  return atan2(y1 - y2, x1 - x2);
}

vector<int> PosicoesRelativasOrdenadas (int i, vector<int>& connections, vector<pair<double, double>> pos){
  double x_ref = pos[i].first;
    double y_ref = pos[i].second;
    sort(connections.begin(), connections.end(), [&](double a, double b) {
        double incl_a = InclinacaoRelativa(pos[a].first, x_ref, pos[a].second, y_ref);
        double incl_b = InclinacaoRelativa(pos[b].first, x_ref, pos[b].second, y_ref);
        return incl_a < incl_b;
    });

    return connections;
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
