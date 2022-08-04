// Computes the sum of the first N integers, counting from 0.
//
#include <cstdio>
#include <cstdlib>
#include <omp.h>

#define N 1000000000

int main()
{

    long long total = 0;

    auto start_time = omp_get_wtime();
    for (int i = 0; i <= N; i++) {
        total += i;
    }
    auto end_time = omp_get_wtime();

    printf("total = %lld\n", total);
    printf("Sequential time = %0.17f\n", (end_time - start_time));

    long long par_total = 0;

    auto par_start_time = omp_get_wtime();

#pragma omp parallel
    {
        long long sub_total = 0; // each thread gets a personal copy of the subtot

#pragma omp for
        for (int i = 0; i <= N; i++) { // Each thread does _part_ of the for loop
            sub_total += i;
        }

#pragma omp critical
        { // We force threads to politely take turns
            par_total += sub_total;
        }
    }
    auto par_end_time = omp_get_wtime();

    printf("Parallel total = %lld\n", par_total);
    printf("Parallel time = %0.17f\n", (par_end_time - par_start_time));

}
