#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 11

int main() {
    unsigned int *vector1, *vector2, *vector3;
    double start_time, end_time;
    
    vector1 = (unsigned int*)malloc(N * sizeof(unsigned int));
    vector2 = (unsigned int*)malloc(N * sizeof(unsigned int));
    vector3 = (unsigned int*)malloc(N * sizeof(unsigned int));

    if (!vector1 || !vector2 || !vector3) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        vector1[i] = rand() % 100;  
        vector2[i] = rand() % 100; 
    }

    printf("Number of threads: %d\n", omp_get_max_threads());

    start_time = omp_get_wtime();

    int i;
    #pragma omp parallel for shared(vector1, vector2, vector3) private(i) schedule(dynamic)
    for (i = 0; i < N; i++) {
        vector3[i] = vector1[i] + vector2[i];
    }

    end_time = omp_get_wtime();

    printf("Results (unsigned int):\n");
    for (int i = 0; i < N; i++) {
        printf("vector3[%d] = %u (%u + %u)\n", 
               i, vector3[i], vector1[i], vector2[i]);
    }

    printf("\nExecution time: %f seconds\n", end_time - start_time);

    free(vector1);
    free(vector2);
    free(vector3);

    return 0;
}