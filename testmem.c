#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

#define OBJECTS 120

/*
Test 1: Make sure the allocations used in workload 1 do not overlap and that data written to each allocation stays unchanged.
 */
void test_workload1(void) {
    int sizes[] = {8, 16, 32, 64, 128, 512, 1024};
    void *ptrs[7];

    printf("Testing workload 1\n");

    for (int i = 0; i < 7; i++) {
        ptrs[i] = malloc(sizes[i]);

        if (ptrs[i] == NULL) {
            printf("FAIL: allocation %d failed\n", i);
            return;
        }

        memset(ptrs[i], i + 1, sizes[i]);
    }

    for (int i = 0; i < 7; i++) {
        unsigned char *data = ptrs[i];

        for (int j = 0; j < sizes[i]; j++) {
            if (data[j] != i + 1) {
                printf("FAIL: data was overwritten in allocation %d\n", i);
                return;
            }
        }
    }

    for (int i = 6; i >= 0; i--) {
        free(ptrs[i]);
    }

    printf("PASS: workload 1\n");
}


/*
Test 2: Allocate 120 small objects, write different values to each, then free them in allocation order.
 */
void test_workload2(void) {
    void *ptrs[OBJECTS];

    printf("Testing workload 2\n");

    for (int i = 0; i < OBJECTS; i++) {
        ptrs[i] = malloc(1);

        if (ptrs[i] == NULL) {
            printf("FAIL: allocation %d failed\n", i);
            return;
        }

        *((char *)ptrs[i]) = i;
    }

    for (int i = 0; i < OBJECTS; i++) {
        if (*((char *)ptrs[i]) != (char)i) {
            printf("FAIL: data was overwritten in allocation %d\n", i);
            return;
        }

        free(ptrs[i]);
    }

    printf("PASS: workload 2\n");
}



//Test 3: Reproduce the general allocation/free pattern from workload 3. Make sure allocated objects can still be accessed correctly.

void test_workload3(void) {
    void *ptrs[OBJECTS] = {NULL};
    int allocated = 0;
    int total_allocations = 0;

    printf("Testing workload 3...\n");

    while (total_allocations < OBJECTS) {
        int choice = rand() % 2;

        if (choice == 0 || allocated == 0) {
            for (int i = 0; i < OBJECTS; i++) {
                if (ptrs[i] == NULL) {
                    ptrs[i] = malloc(1);

                    if (ptrs[i] == NULL) {
                        printf("FAIL: allocation failed\n");
                        return;
                    }

                    *((char *)ptrs[i]) = (char)i;

                    allocated++;
                    total_allocations++;
                    break;
                }
            }
        } else {
            int index = rand() % OBJECTS;

            while (ptrs[index] == NULL) {
                index = (index + 1) % OBJECTS;
            }

            free(ptrs[index]);
            ptrs[index] = NULL;
            allocated--;
        }
    }

    for (int i = 0; i < OBJECTS; i++) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }

    printf("PASS: workload 3\n");
}



//Test 4: Test the more complex allocation/free pattern from workload 4. This also exercises reuse of freed space.
 
void test_workload4(void) {
    void *ptrs[50];

    printf("Testing workload 4...\n");

    for (int i = 0; i < 50; i++) {
        ptrs[i] = malloc((i + 1) * 8);

        if (ptrs[i] == NULL) {
            printf("FAIL: allocation %d failed\n", i);
            return;
        }
    }

    for (int i = 0; i < 50; i += 2) {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }

    for (int i = 0; i < 25; i++) {
        void *p = malloc(16);

        if (p == NULL) {
            printf("FAIL: reuse allocation %d failed\n", i);
            return;
        }

        free(p);
    }

    //Free the remaining original allocations. 
    for (int i = 1; i < 50; i += 2) {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }

    printf("PASS: workload 4\n");
}



//Test 5: Repeatedly allocate several objects and then free them all.
void test_workload5(void) {
    void *ptrs[10];

    printf("Testing workload 5...\n");

    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc(200);

        if (ptrs[i] == NULL) {
            printf("FAIL: allocation %d failed\n", i);
            return;
        }

        memset(ptrs[i], i + 1, 200);
    }

    for (int i = 0; i < 10; i++) {
        free(ptrs[i]);
    }

    printf("PASS: workload 5\n");
}


int main(void) {
    srand(42);

    test_workload1();
    test_workload2();
    test_workload3();
    test_workload4();
    test_workload5();

    printf("All memgrind tests completed.\n");

    return 0;
}