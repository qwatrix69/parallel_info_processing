#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define M 7
#define N 5

int main(int argc, char** argv) {
    int rank, size;
    int *sendbuf = NULL, *recvbuf = NULL;                   

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int block_size = (M * N) / size;
    recvbuf = (int*)malloc(block_size * sizeof(int));

    if (rank == 0) {
        sendbuf = (int*)malloc(M * N * sizeof(int));
        for (int i = 0; i < M * N; i++) {
            sendbuf[i] = i + 1; 
        }

        printf("The original vector (M=%d, N=%d):\n", M, N);
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++)
                printf("%3d ", sendbuf[i * N + j]);
            printf("\n");
        }
        printf("\n");
    }

    MPI_Scatter(
        sendbuf,                
        block_size,            
        MPI_INT,               
        recvbuf,              
        block_size,           
        MPI_INT,             
        0,                   
        MPI_COMM_WORLD
    );

    printf("The process %d received %d elements:\n", rank, block_size);
    for (int i = 0; i < block_size; i++)
        printf("%d ", recvbuf[i]);
    printf("\n\n");

    free(sendbuf);
    free(recvbuf);
    MPI_Finalize();
    return 0;
}