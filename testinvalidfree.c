#include <stdio.h>
#include "mymalloc.h"

int main(void) {

    int x = 42;

    printf("Testing\n");

    free(&x);

    printf("FAIL: invalid pointer was not detected\n");

    return 1;
}

