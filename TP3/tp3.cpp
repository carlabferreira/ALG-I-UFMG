//---------------------------------------------------------------------
// Arquivo	: tp3.cpp
// Conteudo	: implementacao do TP3
// Autor	: Carla Beatriz Ferreira (2022097470)
// Historico:   2023-05-11 - arquivo criado
//              2024-07-31 - criação das estruturas de dados a serem utilizadas
//              2024-08-01 - finalizacao da leitura da entrada com LeSecoesEManobras()
//              2024-08-03 - CalculaCombinacoesDasManobras utilizando principios de Programação Dinamica
//              2024-08-04 - Mudanças em CalculaCombinacoesDasManobras
//              2024-08-10 - Correção de problemas - iniciação do vector incorreta
//              2024-08-11 - Implementação da função auxiliar CalculaTempoGasto separadamente
//              2024-08-15 - PrintaCombinacaoFinal finalizada
//              2024-08-16 - Correção de problemas e testes extras
//              2024-08-21 - Finalização e remoção de comentários e partes desnecessarias
//----------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <algorithm> //para max
#include <climits> // Necessário para LLONG_MAX

using namespace std;

// DEFINICAO DAS ESTRUTURAS DE DADOS E LIMITES UTILIZADOS
const long long int LINF = LLONG_MAX;

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
void CalculaCombinacoesDasManobras(vector<manobra>& manobras, vector<vector<long long int>>& combinacaoManobras, long long int n);
long long int PD(vector<secao>& secoes, vector<manobra>& manobras, vector<vector<long long int>>& combinacaoManobras, vector<vector<pair<long long int, long long int>>>& memoizationTable, long long int maskAnterior, long long int secaoAtual);
long long int CalculaTempoGasto(vector<manobra>& manobras, long long int mask);
void PrintaCombinacaoFinal(vector<vector<pair<long long int, long long int>>>& memoizationTable);
long long int ContaBits1(long long int N);

int main (){
    try{
        long long int N, K;
        cin >> N >> K;
        vector<secao> secoes(N);
        vector<manobra> manobras(K);

        // ------------- PARTE 1 - LEITURA DAS INFORMACOES ------------- 
        LeSecoesEManobras(N, K, secoes, manobras);

        // ------------- PARTE 2 - CALCULO DA PONTUAÇÃO PARA AS COMBINAÇÕES DE MANOBRAS ------------- 
        // ------------- UTILIZANDO PROGRAMAÇÃO DINAMICA ------------- 
        vector<vector<long long int>> combinacaoManobras(1 << K, vector<long long int>(1 << K));
        vector<vector<pair<long long int, long long int>>> memoizationTable (
            N, vector<pair<long long int, long long int>>(1 << K, {-LINF, 0})
        );
        CalculaCombinacoesDasManobras(manobras, combinacaoManobras, K); //faz uma "tabela" de todas as combinações de manobras e suas pontuações
        long long int pontuacaoMaxima = PD(secoes, manobras, combinacaoManobras, memoizationTable, 0, 0); //utiliza programacao dinamica (PD) para calcular a mior pontuação possivel, utilizando a 'tabela' das combinações

        // ------------- PARTE 3 - EXIBIÇÃO DO RESULTADO FINAL ------------- 
        cout << pontuacaoMaxima << endl;
        PrintaCombinacaoFinal(memoizationTable);

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

void CalculaCombinacoesDasManobras(vector<manobra>& manobras, vector<vector<long long int>>& combinacaoManobras, long long int n){
    // Calcula as combinações de todas as manobras
    for (long long int i = 0; i < (1 << n); i++) {
        combinacaoManobras[i][0] = 0;
        for (long long int j = 1; j < (1 << n); j++) {
            long long int somaPontuacoes = 0, qtddManobras = 0;
            for (long long int k = 0; k < n; k++) {
                long long int maskK = 1LL << k; // cria uma máscara de bits onde apenas o bit na posição k é definido como 1, e todos os outros bits são definidos como 0
                if (j & maskK) { // Verifica se a manobra k está no subconjunto j
                    if (i & maskK) somaPontuacoes += manobras[k].pontuacaoBase / 2; // Repetida em i
                    else somaPontuacoes += manobras[k].pontuacaoBase; // Não repetida
                    qtddManobras++;
                }
            }
            combinacaoManobras[i][j] = somaPontuacoes * qtddManobras;
        }
    }
}

long long int PD(vector<secao>& secoes, vector<manobra>& manobras, vector<vector<long long int>>& combinacaoManobras, vector<vector<pair<long long int, long long int>>>& memoizationTable, long long int maskAnterior, long long int secaoAtual) {
    // Caso base: todas as seções foram processadas
    if (secaoAtual == secoes.size()) return 0;

    // Verifica se o valor já foi memorizado
    auto& entradaMemo = memoizationTable[secaoAtual][maskAnterior]; // combinação atual de seção e máscara.
    if (entradaMemo.first != -LINF) return entradaMemo.first;

    long long int pontuacaoMaxima = 0;

    // Itera sobre todas as combinações de manobras para a seção atual
    for (long long int i = 0; i < combinacaoManobras.size(); i++) {
        long long int tempoUsado = CalculaTempoGasto(manobras, i);

        // Se o tempo usado excede o limite da seção, pule essa combinação
        if (tempoUsado > secoes[secaoAtual].tempoTravessia) continue;

        // Chamada recursiva para calcular a pontuação da próxima seção
        long long int nextScore = PD( secoes, manobras, combinacaoManobras, memoizationTable, i, secaoAtual + 1);
        
        // Calcula a pontuação total para a combinação atual
        long long int pontuacaoAtual = combinacaoManobras[maskAnterior][i] * secoes[secaoAtual].bonificacao + nextScore;

        // Atualiza a pontuação máxima se a pontuação atual for maior
        if (pontuacaoAtual > pontuacaoMaxima) {
            pontuacaoMaxima = pontuacaoAtual;
            entradaMemo.second = i;  // Armazena o índice da melhor combinação para reconstrução
        }
    }

    // Memoriza a pontuação máxima e retorna
    entradaMemo.first = pontuacaoMaxima;
    return pontuacaoMaxima;
}

long long int CalculaTempoGasto(vector<manobra>& manobras, long long int mask){
    long long int tempoGasto = 0, i = 0;
    while(mask){
        if(mask & 1) tempoGasto += manobras[i].tempoExecucao;
        mask >>= 1; 
        i++;
    }
    return tempoGasto;
}

void PrintaCombinacaoFinal(vector<vector<pair<long long int, long long int>>>& memoizationTable){
    long long int maskAtual = 0; // Inicializa a máscara de manobras atual como 0
    for (int secao = 0; secao < memoizationTable.size(); secao++) {
        long long int maskManobras = memoizationTable[secao][maskAtual].second; // Obtém a máscara de manobras para a seção atual
        cout << ContaBits1(maskManobras) << " "; // Imprime o número de manobras selecionadas
        
        // Imprime os índices das manobras selecionadas
        for (int bit = 0; maskManobras > 0; maskManobras >>= 1, bit++) {
            if (maskManobras & 1) // Se o bit mais baixo é 1, imprime o índice da manobra
                cout << bit + 1 << " ";
        }
        cout << endl; // Nova linha após a lista de manobras

        maskAtual = memoizationTable[secao][maskAtual].second; // Atualiza a máscara de manobras para a próxima seção
    }
}

long long int ContaBits1(long long int N){
    long long int resp = 0;
    while(N){
        resp += N & 1;
        N >>= 1;
    }
    return resp;
}
