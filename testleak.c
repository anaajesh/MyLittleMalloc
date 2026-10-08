#include <stdio.h>
#include "mymalloc.h"

int main(void) {

    int *a = malloc(sizeof(int));
    char *b = malloc(20);
    double *c = malloc(sizeof(double));

    if (a == NULL || b == NULL || c == NULL) {
        printf("FAIL: malloc failed\n");
        return 1;
    }

    *a = 42;
    *c = 3.14;

    printf("Allocated three objects without freeing them.\n");
    printf("Leak detector should report the leaked objects at exit.\n");

    return 0;
}
