// Computes the sum of the first N integers, counting from 0.
//
#include <cstdio>
#include <cstdlib>
#include <omp.h>

#define N 1000000000

int main()
{

    long long total = 0;

    printf("sizeof(total) = %zu\n", sizeof(total));
    printf("sizeof(long) = %zu\n", sizeof(long));
    printf("sizeof(int) = %zu\n", sizeof(int));

    // total = N * (N+1) / 2

    auto start_time = omp_get_wtime();
    for (int i = 0; i <= N; i++) {
        total += i;
    }
    auto end_time = omp_get_wtime();

    printf("total = %lld\n", total);
    printf("Sequential time = %0.17f\n", (end_time - start_time));

    long long par_total = 0;

    auto par_start_time = omp_get_wtime();

#pragma omp parallel for
    for (int i = 0; i <= N; i++) {
        auto reg = par_total;
        reg = reg + i;
        par_total = reg;
        // par_total += i;
    }
    
    auto par_end_time = omp_get_wtime();

    printf("Parallel total = %lld\n", par_total);
    printf("Parallel time = %0.17f\n", (par_end_time - par_start_time));

}
