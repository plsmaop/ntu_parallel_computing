#include "utils.h"

#define ASSUMED_CACHE_LINE_SIZE 16

int ret[128];
int run(int n, int key) {
    f(n, key, ret, ret+ASSUMED_CACHE_LINE_SIZE, ret+2*ASSUMED_CACHE_LINE_SIZE, ret+3*ASSUMED_CACHE_LINE_SIZE);
    int sum = 0;
    for (int i = 0; i < 4; i++) sum += ret[i*ASSUMED_CACHE_LINE_SIZE];
       
    return sum;
}
