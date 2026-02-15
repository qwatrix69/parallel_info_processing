#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 3
#define M 10

void print_matrix(const char* title, unsigned int matrix[N][M], int rank) {
    if (rank == 0) {
        printf("\n%s\n", title);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                printf("%4u ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void sort_descending(unsigned int *arr, int len) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                unsigned int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    unsigned int matrix[N][M];
    unsigned int *linearized_matrix;
    unsigned int *local_data;
    int sort_type;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (argc < 2) {
        if (rank == 0) {
            printf("Usage: %s --rows (sort columns) or --lines (sort rows)\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }
    
    if (strcmp(argv[1], "--lines") == 0) {
        sort_type = 0;
        if (rank == 0) printf("Mode: sorting by rows\n");
    } 
    else if (strcmp(argv[1], "--rows") == 0) {
        sort_type = 1;
        if (rank == 0) printf("Mode: sorting by columns\n");
    }
    else {
        if (rank == 0) {
            printf("Unknown option: %s\n", argv[1]);
            printf("Use --rows (sort columns) or --lines (sort rows)\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    if (rank == 0) {
        srand(time(NULL));
        printf("Generating matrix %d x %d\n", N, M);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                matrix[i][j] = rand() % 100;
            }
        }
        print_matrix("Initial matrix:", matrix, rank);
        
        linearized_matrix = (unsigned int*)malloc(N * M * sizeof(unsigned int));
        
        if (sort_type) {
            for (int j = 0; j < M; j++) {
                for (int i = 0; i < N; i++) {
                    linearized_matrix[j * N + i] = matrix[i][j];
                }
            }
        } else {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    linearized_matrix[i * M + j] = matrix[i][j];
                }
            }
        }
    }
    
    MPI_Bcast(&sort_type, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int total_blocks = sort_type ? M : N;
    int block_size = sort_type ? N : M;
    
    int blocks_per_process = total_blocks / size;
    int remainder = total_blocks % size;
    
    int *sendcounts = (int*)malloc(size * sizeof(int));
    int *displs = (int*)malloc(size * sizeof(int));
    
    displs[0] = 0;
    for (int i = 0; i < size; i++) {
        sendcounts[i] = (blocks_per_process + (i < remainder ? 1 : 0)) * block_size;
        if (i > 0) {
            displs[i] = displs[i - 1] + sendcounts[i - 1];
        }
    }
    
    int local_blocks_count = sendcounts[rank] / block_size;
    local_data = (unsigned int*)malloc(sendcounts[rank] * sizeof(unsigned int));
    
    MPI_Scatterv(
        rank == 0 ? linearized_matrix : NULL,
        sendcounts,
        displs,
        MPI_UNSIGNED,
        local_data,
        sendcounts[rank],
        MPI_UNSIGNED,
        0,
        MPI_COMM_WORLD
    );
    
    for (int b = 0; b < local_blocks_count; b++) {
        unsigned int block[block_size];
        for (int k = 0; k < block_size; k++) {
            block[k] = local_data[b * block_size + k];
        }
        
        sort_descending(block, block_size);
        
        for (int k = 0; k < block_size; k++) {
            local_data[b * block_size + k] = block[k];
        }
    }
    
    MPI_Gatherv(
        local_data,
        sendcounts[rank],
        MPI_UNSIGNED,
        rank == 0 ? linearized_matrix : NULL,
        sendcounts,
        displs,
        MPI_UNSIGNED,
        0,
        MPI_COMM_WORLD
    );
    
    
    if (rank == 0) {
        if (sort_type) {
            for (int j = 0; j < M; j++) {
                for (int i = 0; i < N; i++) {
                    matrix[i][j] = linearized_matrix[j * N + i];
                }
            }
        } else {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    matrix[i][j] = linearized_matrix[i * M + j];
                }
            }
        }
        
        print_matrix("Sorted matrix:", matrix, rank);
        printf("Blocks distribution: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", sendcounts[i] / block_size);
        }
        printf("\n");
        
        free(linearized_matrix);
    }
    
    free(sendcounts);
    free(displs);
    free(local_data);
    
    MPI_Finalize();
    return 0;
}