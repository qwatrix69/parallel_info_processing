#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define M 7
#define N 5

int main(int argc, char** argv) {
    int rank, size;
    int *sendbuf = NULL, *recvbuf = NULL;
    int *sendcounts, *displs;   
    int base, rest;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    sendcounts = (int*)malloc(size * sizeof(int));
    displs = (int*)malloc(size * sizeof(int));

    base = (M * N) / size;  
    rest = (M * N) % size;  

    displs[0] = 0;
    for (int i = 0; i < size; i++) {
        sendcounts[i] = base + (i < rest ? 1 : 0);
        if (i > 0)
            displs[i] = displs[i - 1] + sendcounts[i - 1];
    }

    recvbuf = (int*)malloc(sendcounts[rank] * sizeof(int));

    if (rank == 0) {
        sendbuf = (int*)malloc(M * N * sizeof(int));
        for (int i = 0; i < M * N; i++)
            sendbuf[i] = i + 1;

        printf("The original vector (M=%d, N=%d):\n", M, N);
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++)
                printf("%3d ", sendbuf[i * N + j]);
            printf("\n");
        }
        printf("\n");
    }

    MPI_Scatterv(
        sendbuf,
        sendcounts,
        displs,
        MPI_INT,
        recvbuf,
        sendcounts[rank],
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    printf("The process %d received %d elements:\n", rank, sendcounts[rank]);
    for (int i = 0; i < sendcounts[rank]; i++)
        printf("%d ", recvbuf[i]);
    printf("\n\n");

    free(sendbuf);
    free(recvbuf);
    free(sendcounts);
    free(displs);
    MPI_Finalize();
    return 0;
}