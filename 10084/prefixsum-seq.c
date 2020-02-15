#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include "utils.h"
 
#define MAXN 10000005
#define MAX_THREAD 6
uint32_t prefix_sum_ans[MAXN];
uint32_t key;
int n;
int subsum_size;
uint32_t base[MAX_THREAD];

void *prefix_sum(void *thread_index) {
    int index = *((int*)thread_index);
    int end = (index+1) * subsum_size <= n ? (index+1)*subsum_size : n;
    uint32_t sum = 0;
    for (int i = index * subsum_size + 1; i <= end; ++i) {
        sum += encrypt(i, key);
        prefix_sum_ans[i] = sum;
    }
    pthread_exit(NULL);
}

void *reduce(void *thread_index) {
    int index = *((int*)thread_index);
    int end = (index+1) * subsum_size <= n ? (index+1)*subsum_size : n;
    for (int i = index * subsum_size + 1; i <= end; ++i) {
        prefix_sum_ans[i] += base[index];
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MAX_THREAD];
    int thread_index[MAX_THREAD];
    while (scanf("%d %" PRIu32, &n, &key) == 2) {
        uint32_t sum = 0;
        subsum_size = (n+MAX_THREAD-1) / MAX_THREAD;
        for (int i = 0; i < MAX_THREAD; ++i) {
            thread_index[i] = i;
            int rtl =
                pthread_create(&threads[i], NULL, prefix_sum, (void *)(thread_index + i));
        }

        pthread_join(threads[0], NULL);
        for (int i = 1; i < MAX_THREAD; ++i) {
            pthread_join(threads[i], NULL);
            int end = i * subsum_size <= n ? i * subsum_size : n;
            base[i] = prefix_sum_ans[end] + base[i-1];
        }
        for (int i = 1; i < MAX_THREAD; ++i) {
            int rtl = 
                pthread_create(&threads[i], NULL, reduce, (void *)(thread_index + i));
        }

        for (int i = 1; i < MAX_THREAD; ++i) {
            pthread_join(threads[i], NULL);
        }

        output(prefix_sum_ans, n);
    }
    return 0;
}
