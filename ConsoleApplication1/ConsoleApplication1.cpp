// Computes the sum of the first N integers, counting from 0.
//
#include <cstdio>
#include <cstdlib>
#include <omp.h>

#define N 2000

double A[N][N], B[N][N], A_Orig[N][N], GroundTruth[N][N];

void Populate(double x[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			x[i][j] = 100.0 * rand() / RAND_MAX;
		}
	}
}

void Copy(double src[N][N], double dest[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			dest[i][j] = src[i][j];
		}
	}
}

void Check(double src[N][N], double dest[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (dest[i][j] != src[i][j]) {
				printf("Error detected at (%d,%d)\n", i, j);
				return;
			}
		}
	}
}

int main()
{
	srand(12);

	Populate(A);
	Copy(A, A_Orig);

	Populate(B);

	auto start_time = omp_get_wtime();

#define OUTER for(int i = 0; i< N; i++)
#define INNER    for(int j = 1; j < N; j++)
#define OP          A[i][j] = A[i][0] + B[i][j]

	OUTER {
		A[i][0] = A[i][0] + B[i][0];
		INNER {
			OP;
		}
	}

	auto end_time = omp_get_wtime();
	Copy(A, GroundTruth);

	printf("Sequential time = %0.17f\n", (end_time - start_time));

	Copy(A_Orig, A);
	auto outer_start_time = omp_get_wtime();

#pragma omp parallel for
	OUTER{
		A[i][0] = A[i][0] + B[i][0];
		INNER {
			OP;
		}
	}

	auto outer_end_time = omp_get_wtime();

	printf("Parallel outer time = %0.17f\n", (outer_end_time - outer_start_time));
	Check(A, GroundTruth);

	Copy(A_Orig, A);
	auto inner_start_time = omp_get_wtime();

	OUTER{
		A[i][0] = A[i][0] + B[i][0];
#pragma omp parallel for
		INNER {
			OP;
		}
	}

	auto inner_end_time = omp_get_wtime();
	printf("Parallel inner time = %0.17f\n", (outer_end_time - outer_start_time));
	Check(A, GroundTruth);
}
