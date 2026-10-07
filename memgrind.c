#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"
#define RUNS 50
#define OBJECTS 120

void workload1(void) {
    int sizes[] = {8, 16, 32, 64, 128, 512, 1024};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    void *ptrs[7];
    for (int i = 0; i < num_sizes; i++) {
        ptrs[i] = malloc(sizes[i]);
    }
    for (int i = num_sizes - 1; i >= 0; i--) {
        if (ptrs[i]) free(ptrs[i]);
    }
}
//https://stackoverflow.com/questions/56966466/memory-coalescing-vs-vectorized-memory-access
void workload2(void) {
    void *ptrs[OBJECTS];
    for (int i = 0; i < OBJECTS; i++) {
        ptrs[i] = malloc(1);
    }
    for (int i = 0; i < OBJECTS; i++) {
        if (ptrs[i]) free(ptrs[i]);
    }
}
void workload3(void) {
    void *ptrs[OBJECTS] = {NULL};
    int allocated = 0;
    int total_alocations = 0;
    while (total_alocations < OBJECTS) {
        int choice = rand() % 2;
        if (choice == 0 || allocated == 0) {
            for (int i = 0; i < OBJECTS; i++) {
                if (ptrs[i] == NULL) {
                    ptrs[i] = malloc(1);
                    allocated++;
                    total_alocations++;
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
}

void workload4(void) {
    void *ptrs[50];
    for (int i = 0; i < 50; i++) {
        ptrs[i] = malloc((i + 1) * 8);
    }
    for (int i = 0; i < 50; i += 2) {
        if (ptrs[i]) {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
    for (int i = 0; i < 25; i++) {
        void *p = malloc(16);
        if (p) free(p);
    }
    for (int i = 1; i < 50; i += 2) {
        if (ptrs[i]) {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
}

void workload5(void) {
    void *ptrs[10];
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc(200);
    }
    for (int i = 0; i < 10; i++) {
        if (ptrs[i]) free(ptrs[i]);
    }
}

double run_test(void (*workload)(void)) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < RUNS; i++) {
        workload();
    }
    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000000.0 +(end.tv_usec - start.tv_usec);
    return elapsed / RUNS;
}
int main(void) {
    srand(42);
    printf("Wokload 1 Avg Time: %.2f \n", run_test(workload1));
    printf("Wokload 2 Avg Time: %.2f \n", run_test(workload2));
    printf("Wokload 3 Avg Time: %.2f \n", run_test(workload3));
    printf("Wokload 4 Avg Time: %.2f \n", run_test(workload4));
    printf("Wokload 5 Avg Time: %.2f \n", run_test(workload5));

    return 0;
}
