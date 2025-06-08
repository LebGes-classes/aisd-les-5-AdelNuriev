#include <iostream>
#include "LazyMinBinomialHeap.h"
using namespace std;
#include <chrono>
#include <algorithm>


const int N_start = 100;
const int N_end = 500;
const int N_increment = 50;
const int Q_repeating = 10;

int main()
{
	LazyMinBinomialHeap* heap = new LazyMinBinomialHeap(1);

	int numberOfIteration = 0;
	int resultsOfEachN[(N_end - N_start) / N_increment + 1] = { 0 };
	for (int k = N_start; k <= N_end; k += N_increment)
	{
		int resultsOfRepeating[Q_repeating] = { 0 };
		int currentArr[N_end + 1] = { 0 };
		for (int p = 0; p < k; p++)
		{
			int x = rand() % 100;
			currentArr[p] = x;
		}
		int j = 0;
		for (int m = 0; m < Q_repeating; m++)
		{
			string i = "" + m;
			j++;
			auto start = chrono::high_resolution_clock::now();

			heap->enqueue(i, j);

			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
			resultsOfRepeating[m] = duration.count();
		}
		sort(begin(resultsOfRepeating), end(resultsOfRepeating));
		int sum = 0;
		for (int i = 0; i < 0.8 * Q_repeating + 1; i++)
		{
			sum += resultsOfRepeating[i];
		}
		resultsOfEachN[numberOfIteration] = sum / (0.8 * Q_repeating + 1);
		numberOfIteration++;
	}


	numberOfIteration = 0;
	for (int i = N_start; i <= N_end; i += N_increment)
	{
		cout << i << "\t" << resultsOfEachN[numberOfIteration] << endl;
		numberOfIteration++;
	}

	return 0;
}
