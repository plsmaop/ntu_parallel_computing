#include <pthread.h>
#include "matrix.h"
#include <assert.h>

#define MAX_THREAD 6
#define MAXN 2048

static unsigned long transposed_B[MAXN][MAXN];

struct arg {
    int base_index;
    int N;
    unsigned long *A;
    unsigned long *B;
    unsigned long *C;
};

void *__multiply(void *arg) {
    struct arg *const __arg = (struct arg *)arg;
    int N = __arg->N;
    int linear_index = N * N;
    int base_index = __arg->base_index;

    int i, j;
    while (base_index < linear_index) {
        i = base_index / N;
        j = base_index % N;
        unsigned long sum = 0;  // overflow, let it go.
        for (int k = 0; k < N; ++k) {
            sum += *(__arg->A + i * MAXN + k) * *(__arg->B + j * MAXN + k);
        }

        *(__arg->C + i * MAXN + j) = sum;
        base_index += MAX_THREAD;
    }

    return NULL;
}

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
    pthread_t threads[MAX_THREAD];
    int thread_index[MAX_THREAD];
    struct arg args[MAX_THREAD];

    // for optimize inner loop
    duplicate_and_transpose(N, B, transposed_B);

    int linear_index = N * N;
    for (int i = 0; i < MAX_THREAD && i < linear_index; ++i) {
        thread_index[i] = i;
        args[i].base_index = i;
        args[i].N = N;
        args[i].A = A;
        args[i].B = transposed_B;
        args[i].C = C;

        int rtl =
            pthread_create(&threads[i], NULL, __multiply, (void *)(args + i));
    }

    for (int i = 0; i < MAX_THREAD && i < linear_index; ++i) {
        pthread_join(threads[i], NULL);
    }
}
