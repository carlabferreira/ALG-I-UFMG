//---------------------------------------------------------------------
// Arquivo	: tp3.cpp
// Conteudo	: implementacao do TP3
// Autor	: Carla Beatriz Ferreira (2022097470)
// Historico:   2023-05-11 - arquivo criado
//              2024-07-31 - criação das estruturas de dados a serem utilizadas
//              2024-08-01 - finalizacao da leitura da entrada com LeSecoesEManobras()
//              2024-08-03 - CalculaCombinacoesDasManobras utilizando principios de Programação Dinamica
//              2024-08-04 - 
//              2024-08-10 - 
//              2024-08-11 - 
//              2024-08-15 - 
//              2024-08-16 - 
//----------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <algorithm> //para max
#include <climits> // Necessário para LLONG_MAX

using namespace std;

// DEFINICAO DAS ESTRUTURAS DE DADOS E LIMITES UTILIZADOS
typedef struct secao {
    long long int bonificacao; //ci
    long long int tempoTravessia; //li
} secao;

typedef struct manobra{
    long long int pontuacaoBase; //pj
    long long int tempoExecucao; //tj
} manobra;

// DEFINICAO DAS FUNCOES UTILIZADAS
void LeSecoesEManobras(long long int N, long long int K, vector<secao> &secoes, vector<manobra> &manobras);
void CalculaCombinacoesDasManobras(const vector<manobra> &manobras, const long long int K, vector<vector<long long int>> &combinacaoManobras);

int main (){
    try{
        long long int N, K;
        cin >> N >> K;
        vector<secao> secoes;
        vector<manobra> manobras;

        // ------------- PARTE 1 - LEITURA DAS INFORMACOES ------------- 
        LeSecoesEManobras(N, K, secoes, manobras);

        // ------------- PARTE 2 - CALCULO DA PONTUAÇÃO PARA AS COMBINAÇÕES DE MANOBRAS ------------- 
        // ------------- UTILIZANDO PROGRAMAÇÃO DINAMICA ------------- 
        vector<vector<long long int>> combinacaoManobras;
        CalculaCombinacoesDasManobras(manobras, K, combinacaoManobras); //faz uma "tabela" de todas as combinações de manobras e suas pontuações

        // ------------- PARTE 3 - EXIBIÇÃO DO RESULTADO FINAL ------------- 
    } catch (...) {
        cout << "Erro inesperado!" << endl;
    }
    return 0;
}

void LeSecoesEManobras(long long int N, long long int K, vector<secao> &secoes, vector<manobra> &manobras){
    for (long long int i = 0; i < N; i++){ //Leitura das informações de todas as secoes
        cin >> secoes[i].bonificacao >> secoes[i].tempoTravessia; 
    }
    for (long long int j = 0; j < K; j++){ //leitura das informações de todas as manobras
        cin >> manobras[j].pontuacaoBase >> manobras[j].tempoExecucao;
    }
}

void CalculaCombinacoesDasManobras(const vector<manobra> &manobras, const long long int K,vector<vector<long long int>> &combinacaoManobras){
    // 2^K combinações possíveis de manobras (usando 1 << K para calcular 2^K) = shift logico
    long long int numCombinacoes = 1 << K; // Total de subconjuntos possíveis
    
    // Redimensiona o vetor de combinações para armazenar [tempo total, pontuação total] para cada subconjunto
    combinacaoManobras.resize(numCombinacoes, vector<long long int>(2, 0)); // Cada entrada terá [tempoTotal, pontuacaoTotal]

    // Para cada máscara (0 até 2^K - 1), calcular tempo e pontuação da combinação correspondente
    for (int mask = 0; mask < numCombinacoes; mask++) {
        long long int tempoTotal = 0;
        long long int pontuacaoTotal = 0;

        // Itera sobre cada manobra para verificar se ela está na combinação representada por 'mask'
        for (int i = 0; i < K; ++i) {
            if (mask & (1 << i)) { // Verifica se o bit i-ésimo está ativado na máscara
                tempoTotal += manobras[i].tempoExecucao;
                pontuacaoTotal += manobras[i].pontuacaoBase;
            }
        }

        // Armazena o tempo e a pontuação total para esta combinação de manobras
        combinacaoManobras[mask][0] = tempoTotal;
        combinacaoManobras[mask][1] = pontuacaoTotal;
    }
}
