#include <stdio.h>
#include "mymalloc.h"


int main() {

    int *p = malloc(sizeof(int));

    if (p == NULL) {
        printf("malloc failed\n");
        return 1;
    }

    printf("malloc worked! p = %p\n", (void *)p);

    *p = 42;

    printf("p contains: %d\n", *p);

    return 0;
}