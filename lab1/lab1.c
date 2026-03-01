#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define M 7
#define N 5

int main(int argc, char** argv) {
    int rank, size;
    int *sendbuf = NULL, *recvbuf = NULL;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != N) {
        if (rank == 0) {
            printf("Error: Number of processes must be equal to N (%d)\n", N);
        }
        MPI_Finalize();
        return 1;
    }

    recvbuf = (int*)malloc(M * sizeof(int));

    if (rank == 0) {
        sendbuf = (int*)malloc(M * N * sizeof(int));
        for (int i = 0; i < M * N; i++) {
            sendbuf[i] = i + 1; 
        }

        printf("Original vector (M=%d):\n", M);
        for (int i = 0; i < M; i++) {
            printf("%d ", sendbuf[i]);
        }
        printf("\n\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    MPI_Scatter(
        sendbuf,                
        M,            
        MPI_INT,               
        recvbuf,              
        M,           
        MPI_INT,             
        0,                   
        MPI_COMM_WORLD
    );

    end_time = MPI_Wtime();
    
    printf("Process %d received %d elements:\n", rank, M);
    for (int i = 0; i < M; i++)
        printf("%d ", recvbuf[i]);
    printf("\n");

    if (rank == 0) {
        printf("\nScatter execution time: %f seconds\n", end_time - start_time);
    }

    if (rank == 0) {
        free(sendbuf);
    }
    free(recvbuf);
    
    MPI_Finalize();
    return 0;
}