#include <iostream>
#include <thread>
#include <chrono>

// Função que será executada
#include <stack>
#include <vector>
using namespace std;

//? ---------------------------
// DEFINICAO DAS MINHAS FUNÇÔES ESPECIFICAS / EXTRAS
void QuicksortRecursivoPuro_Carla (vector<int> &V, int esq, int dir);
void QuicksortRecursivoOrdenacaoInsercao_Carla (vector<int> &V, int esq, int dir);
void InsertionSort(vector<int>& v, int esq, int dir);
int Particao(vector<int>& V, int esq, int dir);
void Mediana_3valores (vector<int> &v, int dir, int esq);
void Troca(int& a, int& b);



//FUNCOES
vector<int> QuicksortRecursivoPuro(const vector<int> arr){
    if (arr.size() <= 1) return arr;
    
    vector<int> vetorOrdenado (arr.begin(), arr.end()); //copia o vetor const recebido para um a ser ordenado
    QuicksortRecursivoPuro_Carla(vetorOrdenado, 0, (vetorOrdenado.size()-1)); //!! menos um pois o tamanho size e o vetor é de 0 a size-1
    return vetorOrdenado;
}

void QuicksortRecursivoPuro_Carla (vector<int> &V, int esq, int dir) {
    if(esq < dir) {
        int indicePivo = Particao(V, esq, dir);
        QuicksortRecursivoPuro_Carla(V, esq, indicePivo - 1);
        QuicksortRecursivoPuro_Carla(V, indicePivo, dir);
    }
}

vector<int> QuicksortRecursivoOrdenacaoInsercao(const vector<int> arr){
    if (arr.size() <= 1) return arr;
    
    vector<int> vetorOrdenado (arr.begin(), arr.end());
    QuicksortRecursivoOrdenacaoInsercao_Carla(vetorOrdenado, 0, (vetorOrdenado.size()-1));
    
    return vetorOrdenado;
}

void QuicksortRecursivoOrdenacaoInsercao_Carla (vector<int> &V, int esq, int dir) {
    if(esq < dir) {
    if ((dir - esq) <= 30) InsertionSort(V, esq, dir); 
    else {
        int indicePivo = Particao(V, esq, dir);
        QuicksortRecursivoOrdenacaoInsercao_Carla(V, esq, indicePivo - 1);
        QuicksortRecursivoOrdenacaoInsercao_Carla(V, indicePivo, dir);
    }
    }

}

vector<int> QuicksortNaoRecursivoPuro(const vector<int> arr){
    if (arr.size() <= 1) return arr;
    
    vector<int> vetorOrdenado (arr.begin(), arr.end());

    // Cria uma pilha para armazenar os limites do subvetor
    stack<pair<int, int>> pilha;
    pilha.push(make_pair(0, vetorOrdenado.size()-1)); // empilha os limites iniciais

    // Enquanto a pilha não estiver vazia
    while (!pilha.empty()) {
        // Desempilha os limites do subvetor atual
        int inicio = pilha.top().first;
        int fim = pilha.top().second;
        pilha.pop();

        // Particiona o subvetor e obtém a posição final do pivô
        int pivoFinal = Particao(vetorOrdenado, inicio, fim);

        // Se houver elementos à esquerda do pivô, empilhe seus limites
        if (pivoFinal - 1 > inicio)
            pilha.push(make_pair(inicio, pivoFinal - 1));
        
        // Se houver elementos à direita do pivô, empilhe seus limites
        if (pivoFinal + 1 < fim)
            pilha.push(make_pair(pivoFinal + 1, fim));
    }
    return vetorOrdenado;
}

vector<int> QuicksortNaoRecursivoOrdenacaoInsercao(const vector<int> arr){
    if (arr.size() <= 1) return arr;

    vector<int> vetorOrdenado (arr.begin(), arr.end());
    if(vetorOrdenado.size() <= 30) {
        InsertionSort(vetorOrdenado, 0, (vetorOrdenado.size()-1));
        return vetorOrdenado;
    }
    
    // Cria uma pilha para armazenar os limites do subvetor
    stack<pair<int, int>> pilha;
    pilha.push(make_pair(0, vetorOrdenado.size()-1)); // empilha os limites iniciais

    // Enquanto a pilha não estiver vazia
    while (!pilha.empty()) {
        // Desempilha os limites do subvetor atual
        int inicio = pilha.top().first;
        int fim = pilha.top().second;
        pilha.pop();
        if (fim - inicio <= 30) InsertionSort(vetorOrdenado, inicio, fim);
        else {
            // Particiona o subvetor e obtém a posição final do pivô
            int pivoFinal = Particao(vetorOrdenado, inicio, fim);

            // Se houver elementos à esquerda do pivô, empilhe seus limites
            if (pivoFinal - 1 > inicio) pilha.push(make_pair(inicio, pivoFinal - 1));
            
            // Se houver elementos à direita do pivô, empilhe seus limites
            if (pivoFinal + 1 < fim) pilha.push(make_pair(pivoFinal + 1, fim));
        }
    }
    return vetorOrdenado;
}

void InsertionSort(vector<int>& V, int esq, int dir){
    for (int i = esq + 1; i <= dir; i++) { 
		
		int j = i;
	
		while (j > esq && V[j] < V[j-1]) {
			int aux = V[j];
			V[j] = V[j - 1];
			V[j - 1] = aux;
			j -= 1;
		}
	
	}	
}

int Particao(vector<int>& V, int esq, int dir) {
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

void Troca(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}


void printaVector (vector<int> V){
    for (int i = 0; i < (int)V.size(); i++){
        cout << V[i] << ' ';
    }
    cout << endl;
}
int main() {
    vector<int> V = {1, 10, 2, 9, 3, 8, 4, 7, 5, 6, 10, 15, 17, 20, 87, 90, 55, 54, 30, 2};
    vector<int> Resp;

    Resp = QuicksortRecursivoPuro (V);
    printaVector(Resp);

    Resp = QuicksortRecursivoOrdenacaoInsercao (V);
    printaVector(Resp);

    Resp = QuicksortNaoRecursivoPuro (V);
    printaVector(Resp);

    Resp = QuicksortNaoRecursivoOrdenacaoInsercao (V);
    printaVector(Resp);

    return 0;
}
