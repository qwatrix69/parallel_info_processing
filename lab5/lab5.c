#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ROWS 90
#define COLS 60
#define MAX_VALUE 100

void bubble_column_sort(unsigned int *col, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (col[j] < col[j + 1]) {
                unsigned int temp = col[j];
                col[j] = col[j + 1];
                col[j + 1] = temp;
            }
        }
    }
}

void save_matrix_full(unsigned int matrix[ROWS][COLS], const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(f, "%u ", matrix[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    printf("Full matrix saved in %s\n", filename);
}

int main() {
    unsigned int matrix[ROWS][COLS];
    
    srand(time(NULL));
    
    printf("Sorting matrix %d x %d\n", ROWS, COLS);
    printf("The number of threads: %d\n", omp_get_max_threads());
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            matrix[i][j] = rand() % MAX_VALUE;
        }
    }
    
    save_matrix_full(matrix, "original.txt");
    
    #pragma omp parallel for
    for (int j = 0; j < COLS; j++) {
        unsigned int col[ROWS];
        for (int i = 0; i < ROWS; i++) col[i] = matrix[i][j];
        bubble_column_sort(col, ROWS);
        for (int i = 0; i < ROWS; i++) matrix[i][j] = col[i];
    }
    
    save_matrix_full(matrix, "sorted.txt");
    
    return 0;
}