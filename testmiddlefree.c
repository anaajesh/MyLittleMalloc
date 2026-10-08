#include <stdio.h>
#include "mymalloc.h"

int main(void) {

    int *p = malloc(5 * sizeof(int));

    if (p == NULL) {
        printf("FAIL: malloc failed\n");
        return 1;
    }

    p[0] = 10;
    p[1] = 20;
    p[2] = 30;

    free(p + 1);

    printf("FAIL: middle pointer was not detected\n");

    return 1;
}