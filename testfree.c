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

    printf("Free worked!\n");

    return 0;
}