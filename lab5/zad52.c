#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <string.h>

#define SIZE 40

#define TEST 0 // 1 - dla testow,  0 - dla oceny automatycznej

void swap(int * a, int * b) {
  int c = * a;
  * a = * b;
  * b = c;
}

void set_perm(const double A[][SIZE], int perm[], size_t n){
    int temp;
    for (size_t i=0; i<n; i++)
        for (size_t j=0; j<n-i-1; j++)
            if (fabs(A[perm[j]][perm[j]])>fabs(A[perm[j+1]][perm[j+1]])){
                temp=perm[j];
                perm[j]=perm[j+1];
                perm[j+1]=temp;
            }
}

void read_vec(double x[], size_t n) {
  for (size_t i = 0; i < n; ++i) scanf("%lf", x++);
}

void print_vec(const double x[], size_t n) {
  for (size_t i = 0; i < n; ++i) printf("%.4f ", x[i]);
  printf("\n");
}

void read_mat(double A[][SIZE], size_t m, size_t n) {
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) scanf("%lf", & A[i][j]);
  }
}

void print_mat(const double A[][SIZE], size_t m, size_t n) {
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) printf("%.4f ", A[i][j]);
    printf("\n");
  }
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

double gauss(double A[][SIZE], double b[], double x[], size_t n, double eps) {
    int indices[n],start_pos=0;
    double det = 1,multiplier,p[n];
    for(int i = 0; i < n; i++) {
        indices[i] = i;
        p[i] = b[i];
    } 

    // Upper triangular matrix
    for(start_pos = 0; start_pos < n; start_pos++){

        for(int j = start_pos + 1; j < n; j++){
            if(fabs(A[indices[start_pos]][start_pos]) < fabs(A[indices[start_pos]][j])){
                swap(&indices[start_pos], &indices[j]);
                det *= -1;
            }
        }

        if(fabs(A[indices[start_pos]][start_pos]) < eps)
            return 0;

        for(int j = start_pos + 1; j < n; j++){
            multiplier = A[indices[j]][start_pos] / A[indices[start_pos]][start_pos];
            for(int k = start_pos; k < n; k++){
                A[indices[j]][k] -= A[indices[start_pos]][k]*multiplier;
            }
            p[indices[j]] -= p[indices[start_pos]]*multiplier;
        }
    }

    for(start_pos = 0; start_pos < n; start_pos++){
        det *= A[indices[start_pos]][start_pos];
    }

    if(det != 0) {
        for (start_pos = (int) n - 1; start_pos >= 0; start_pos--) {
            double sum = p[indices[start_pos]];
            for (int j = (int) n - 1; j > start_pos; j--) {
                sum -= x[j] * A[indices[start_pos]][j];
            }
            x[start_pos] = sum / A[indices[start_pos]][start_pos];
        }
    }

    return det;
}


// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
	int indices[n];
	int swapped, absolute_value, max_indx, swaps = 0,start_pos;
	double ratio, det = 1, max_val;

	for (int i = 0; i < n; i++) indices[i] = i;

	for (int i = n; i < 2 * n; i++) A[i - n][i] = 1;

	for (start_pos = 0; start_pos < n - 1; start_pos++){
		max_val = 0;
		max_indx = 0;
		for (int r = start_pos; r < n; r++){
			absolute_value = fabs(A[indices[r]][start_pos]);
			if (absolute_value > max_val){
				max_val = absolute_value;
				max_indx = r;
			}
		}
		swap(&indices[start_pos],&indices[max_indx]);

		if (swapped != indices[start_pos]) swaps++;

		if (max_val < eps) return 0;

		for (int i = start_pos + 1; i < n; i++){
			ratio = A[indices[i]][start_pos] / A[indices[start_pos]][start_pos];

			for (int k = 0; k < 2 * n; k++){ // zmiana wraz z kolumną uzupełnioną
				A[indices[i]][k] -= A[indices[start_pos]][k] * ratio;
			}
		}

	}
	for (int i = 0; i < n; i++){
		det *= A[indices[i]][i];
	}

	if (swaps % 2 != 0) det = -det;

	ratio = A[indices[n - 1]][n - 1];
	for (int start_pos = n - 1; start_pos < 2 * n; start_pos++) A[indices[n - 1]][start_pos] /= ratio;

	for (int r = n - 1; r > 0; r--){
		for (int rem = r - 1; rem > -1; rem--){
			ratio = A[indices[rem]][r] / A[indices[r]][r];

			for (int start_pos = r; start_pos < 2 * n; start_pos++) A[indices[rem]][start_pos] -= ratio * A[indices[r]][start_pos];
		}

		ratio = A[indices[r - 1]][r - 1];
		for (int start_pos = r - 1; start_pos < 2 * n; start_pos++) A[indices[r - 1]][start_pos] /= ratio;
	}

	for (int i = 0; i < n; i++){
		for (int j = n; j < 2 * n; j++) B[i][j - n] = A[indices[i]][j];
	}

	return det;
}


int main(void) {

  double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
  double b[SIZE], x[SIZE], det, eps = 1.e-13;
  int to_do;
  size_t m, n, p, q;
  if (TEST) printf("Wpisz numer testu ");
  scanf("%d", & to_do);
  switch (to_do) {
  case 4:
    if (TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
    scanf("%d", & n);
    if (TEST) printf("Wpisz macierz A (wierszami): ");
    read_mat(A, n, n);
    if (TEST) printf("Wpisz wektor b: ");
    read_vec(b, n);
    det = gauss(A, b, x, n, eps);
    printf("%.4f\n", det);
    if (det) print_vec(x, n);
    break;
  case 5:
    if (TEST) printf("Wpisz rozmiar macierzy n = ");
    scanf("%d", & n);
    if (TEST) printf("Wpisz elementy macierzy (wierszami): ");
    read_mat(A, n, n);
    det = matrix_inv(A, B, n, eps);
    printf("%.4f\n", det);
    if (det) print_mat(B, n, n);
    break;
  default:
    printf("NOTHING TO DO FOR %d\n", to_do);
  }
  return 0;
}