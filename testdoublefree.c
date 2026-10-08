#include <stdio.h>
#include "mymalloc.h"

int main(void) {

    int *p = malloc(sizeof(int));

    if (p == NULL) {
        printf("FAIL: malloc failed\n");
        return 1;
    }

    *p = 42;

    printf("first free passed \n");

    free(p);

    printf("second free passed\n");

    free(p);

    printf("FAIL: double free was not detected\n");

    return 1;
}

