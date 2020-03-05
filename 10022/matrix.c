#include "matrix.h"
#include <omp.h>

#define MAXN 2048

static unsigned long transposed_B[MAXN][MAXN];

void duplicate_and_transpose(int N, unsigned long src[][MAXN],
                             unsigned long dest[][MAXN]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            dest[i][j] = src[j][i];
        }
    }
}

void multiply(int N, unsigned long A[][MAXN], unsigned long B[][MAXN],
              unsigned long C[][MAXN]) {
    // for optimize inner loop
    duplicate_and_transpose(N, B, transposed_B);

    int linear_index = N * N;
#pragma omp parallel for
    for (int ind = 0; ind < linear_index; ++ind) {
        int i = ind / N;
        int j = ind % N;
        unsigned long sum = 0;  // overflow, let it go.
        for (int k = 0; k < N; ++k) {
            sum += A[i][k] * transposed_B[j][k];
        }

        C[i][j] = sum;
    }
}
