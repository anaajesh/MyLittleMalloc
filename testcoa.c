#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

int main(void) {

    printf("Testing coalescing...\n");

    // Allocate three adjacent chunks
    void *a = malloc(100);
    void *b = malloc(100);
    void *c = malloc(100);

    if (a == NULL || b == NULL || c == NULL) {
        printf("FAIL: Initial allocations failed.\n");
        return 1;
    }

    printf("Initial allocations: PASS\n");

    free(a);
    free(b);
    free(c);

    //testing big chunk after combinin other chunks -> shld work now if coalesced properly
    void *big = malloc(300);

    if (big == NULL) {
        printf("FAIL: Coalescing is not working\n");
        return 1;
    }

    printf("Triple coalescing passed\n");

    free(big);

    printf("YUP, YOUR COALESCING IS WORKING!\n");

    return 0;
}