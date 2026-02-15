#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 40 
#define N 6  

void print_vector(const char* prefix, unsigned int* vec, int size, int process) {
    printf("%s %d :\n ", prefix, process);
    printf("        ");
    for (int i = 0; i < size; i++) {
        printf("%3u ", vec[i]);
        if ((i + 1) % 10 == 0 && i < size - 1) {
            printf("\n         ");
        }
    }
    printf("\n");
}

int main(int argc, char** argv) {
    int rank, size;
    unsigned int *local_vector;     
    unsigned int *result_vector;  
    double start_time, end_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size != N) {
        if (rank == 0) {
            printf("Error: the program must be started on the %d processes\n", N);
        }
        MPI_Finalize();
        return 1;
    }

    srand(time(NULL) + rank * 100);
    local_vector = (unsigned int*)malloc(M * sizeof(unsigned int));
    
    for (int i = 0; i < M; i++) {
        local_vector[i] = rand() % 256;
    }
    
    printf("\nProcess %d generate the vector\n", rank);
    print_vector("The vector of the process", local_vector, M, rank);
    
    if (rank == 0) {
        result_vector = (unsigned int*)malloc(M * sizeof(unsigned int));
    }
    
    MPI_Reduce(
        local_vector,      
        result_vector,     
        M,                 
        MPI_UNSIGNED,       
        MPI_BAND,         
        0,                 
        MPI_COMM_WORLD
    );
    
    if (rank == 0) {
        print_vector("\nResult", result_vector, M, 0);
    }
    
    free(local_vector);
    if (rank == 0) {
        free(result_vector);
    }
    
    MPI_Finalize();
    return 0;
}