#include<iostream>
#include<cstdlib>
#include<fstream>
#include<iomanip>
#include<math.h>
#include <cmath>

using namespace std;

bool GaussPP(int how_many, double** tab) {
	for (int i = 0; i < how_many; i++) {
		bool ifchg = 0;
		int max = i;
		for (int j = i; j < how_many; j++) {
			if (abs(tab[max][i]) < abs(tab[j][i])) {
				ifchg = 1;
				max = j;
			}
		}
		if (ifchg) {
			for (int j = i; j <= how_many; j++) {
				double temp = tab[i][j];
				tab[i][j] = tab[max][j];
				tab[max][j] = temp;
			}
		}
		for (int j = i; j < how_many - 1; j++) {
			if (tab[i][i] != 0) {
				double mnoznik = (double)tab[j + 1][i] / tab[i][i];
				for (int k = i; k < how_many + 1; k++) {
					tab[j + 1][k] = (double)tab[j + 1][k] - (double)mnoznik * (double)tab[i][k];
				}
			}
			else {
				cout << "Wystêpuje 0 na przek¹tnej - nie da siê obliczyæ" << endl;
				return 0;
			}
		}
	}
	return 1;
}

void x(int how_many, double* tabx, double ** tab) {
	for (int i = how_many - 1; i >= 0; i--) {
		double suma = 0.0;
		for (int k = i + 1; k < how_many; k++) {
			suma = suma + tab[i][k] * tabx[k];
		}
		tabx[i] = (double)(tab[i][(how_many)] - suma) / tab[i][i];
	}
}
