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

    // Free all three chunks
    free(a);
    free(b);
    free(c);

    /*
     * If coalescing works, the three adjacent free chunks
     * should have been combined into one large free chunk.
     *
     * We should therefore be able to allocate a chunk
     * that is larger than any one of the original chunks.
     */
    void *big = malloc(300);

    if (big == NULL) {
        printf("FAIL: Coalescing is not working correctly.\n");
        return 1;
    }

    printf("Triple coalescing: PASS\n");

    free(big);

    printf("YUP, YOUR COALESCING IS WORKING!\n");

    return 0;
}