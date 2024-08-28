#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

void geraAleatorios(int qtddNumeros, ofstream&  arquivo, int i);
void criarArquivos(int qtddArquivos, int qtddNumeros);
vector<int> leArquivo(string nomeArquivo);
