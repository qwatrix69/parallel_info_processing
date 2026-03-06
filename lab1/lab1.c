#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    int M = 7;
    int N = 5;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size != N) {
        MPI_Finalize();
        return 1;
    }
    
    double start_time, end_time;
    int* send_vector = NULL;
    int* recv_vector = NULL;
    
    recv_vector = (int*)malloc(M * sizeof(int));
    
    if (rank == 0) {
        send_vector = (int*)malloc(M * N * sizeof(int));

        printf("Original vector (M=%d):\n", M);
        
        for (int i = 0; i < M; i++) {
            int value = i * i;
            printf("%d ", value);
            for (int j = 0; j < N; j++) {
                send_vector[j * M + i] = value;
            }
        }
        printf("\n\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    
    MPI_Scatter(send_vector, M, MPI_INT,
                recv_vector, M, MPI_INT,
                0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        end_time = MPI_Wtime();
    }
    
    printf("Process %d received vector: ", rank);
    for (int i = 0; i < M; i++) {
        printf("%d ", recv_vector[i]);
    }
    printf("\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("\nExecution time: %f seconds\n", end_time - start_time);
        free(send_vector);
    }
    
    free(recv_vector);
    
    MPI_Finalize();
    return 0;
}