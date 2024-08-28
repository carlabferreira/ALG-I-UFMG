#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

long long int fibonacciIterativo(int tempoLimite);
int fibonacciRecursivo(int n);

void SegundosFibonacci(void);
void ContaTempoRecursivo (int tempoLimite);
void ComparacaoFibonacci(void);

int main(void){

    int op;

    cout << "Selecione a opcao:" << endl;
    cout << "(0) Gerar tabela definida (15s, 30s, 45s, 60s, 75s, 90s, 105s e 120s)" << endl
         << "(1) Teste com entrada de segundos" << endl
         << "(2) Finalizar" << endl;

    cin >> op;

    if (op == 0) ComparacaoFibonacci();
    else if (op == 1) SegundosFibonacci();
    else if (op == 2) {cout << "Finalizado" << endl; return 0;}
    else cout << "opcao inválida!" << endl;

    return 0;
}

void ComparacaoFibonacci(void){
    vector<int> V = {15, 30, 45, 60, 75, 90, 105, 120}; // 
    cout << " ----------------------------------------------------------------------- " << endl;
    cout << "|\t Segundos \t|\t #Iterativo \t|\t #Recursivo \t|" << endl;
    for (int i = 0; i < V.size(); i++){
        //cout << "V[" << i << "] = " << V[i] << ' '; 
        cout << "|\t " << V[i] << "\t\t|\t " << fibonacciIterativo(V[i]) << "\t|\t ";
        ContaTempoRecursivo(V[i]);
        cout << " \t\t|" << endl;
    }
    cout << " ----------------------------------------------------------------------- " << endl;
}

long long int fibonacciIterativo(int tempoLimite) {
    unsigned long long int primeiro = 0, segundo = 1, proximo;
    long long int n = 0;

    // Obtém o tempo inicial
    auto inicio = chrono::steady_clock::now();

    // Executa o cálculo da sequência de Fibonacci enquanto o tempo não atingir o limite
    while (true) {
        auto atual = chrono::steady_clock::now();
        auto duracao = chrono::duration_cast<chrono::seconds>(atual - inicio).count();

        if (duracao >= tempoLimite)
            break;

        if (n <= 1)
            proximo = n;
        else {
            proximo = primeiro + segundo;
            primeiro = segundo;
            segundo = proximo;
        }
        n++;
    }
    return n;
}

int fibonacciRecursivo(int n) {
    if (n <= 1)
        return n;
    return fibonacciRecursivo(n - 1) + fibonacciRecursivo(n - 2);
}

void SegundosFibonacci(void) {
    cout << "Digite a quantidade de segundos a ser executado: " << endl;
    int tempoLimite;
    cin >> tempoLimite;

    int resultado = fibonacciIterativo(tempoLimite);
    cout << "Quantidade de números de Fibonacci Iterativo calculado em " << tempoLimite << " segundos: " << resultado << endl;

    cout << "Quantidade de números de Fibonacci Recursivo calculado em " << tempoLimite << " segundos: " ;
    ContaTempoRecursivo(tempoLimite);
    cout << endl;

}

void ContaTempoRecursivo (int tempoLimite) {
    //? funciona esquisito tbm, reinicia o tempo e faz dnv ate aquele outro - pode ser considerado correto de acordo com a c=documentação
    
    //vector<int> V;
    // Inicia uma thread com a função fibonacciRecursivo
    std::thread t([&tempoLimite](){
        int n = 0;
        auto inicio = chrono::steady_clock::now();
        bool tempoEsgotado = false;

        // Calcula os termos da sequência de Fibonacci até que o tempo limite seja atingido
        while (!tempoEsgotado) {
            auto atual = chrono::steady_clock::now();
            auto duracao = chrono::duration_cast<chrono::seconds>(atual - inicio).count();

            if (duracao >= tempoLimite)
                tempoEsgotado = true;
            fibonacciRecursivo(n);
            //cout << fibonacciRecursivo(n) << " "; // Exibe o resultado
            //cout.flush(); // Limpa o buffer de saída
            n++;
        }
        //cout << "Número máximo de Fibonacci Recursivo calculado em " << tempoLimite << " segundos: " << n << endl;
        cout << n;
    });
    
    // Aguarda até que a thread termine ou até que o tempo limite seja atingido
    t.join();
}
