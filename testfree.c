#include <stdio.h>
#include "mymalloc.h"

int main() {

    int *a = malloc(sizeof(int));

    if (a == NULL) {
        printf("malloc failed\n");
        return 1;
    }

    *a = 42;

    printf("Before free: %d\n", *a);

    free(a);

    int *b = malloc(sizeof(int));

    if (b == NULL) {
        printf("FAIL: memory was not reusable\n");
        return 1;
    }

    *b = 100;

    printf("After free, new allocation worked: %d\n", *b);

    free(b);

    printf("Free worked!\n");

    return 0;
}