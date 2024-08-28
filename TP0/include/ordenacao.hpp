#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <random>

using namespace std;

void QuicksortRecursivoPuro (vector<int> &values, int left, int right);

void QuicksortRecursivo_InsertionSort(vector<int> &values, int left, int right);

void QuicksortNaoRecursivoPuro (vector<int>& V);

void QuicksortNaoRecursivo_InsertionSort (vector<int>& V);

void InsertionSort(vector<int>& V, int esq, int dir);
void Mediana_3valores (vector<int> &V, int right, int left);
void Troca(int& a, int& b);
int particao(vector<int>& V, int esq, int dir); //todo verificar
