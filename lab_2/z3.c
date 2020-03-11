#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void get_device_info(int* rank, int*size)
{
    MPI_Comm_rank (MPI_COMM_WORLD, rank);  /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, size);  /* get number of processes */
}

void Scatter(int* send_data, int* recv_data, int count) {
    int size, rank, i;
    int type_size;

    MPI_Datatype type = MPI_INT;
    MPI_Type_size(type, &type_size);

    //MPI_Status status;
    get_device_info(&rank, &size);

    if (rank == 0){
        for (i = 1; i < size; i++){
            MPI_Send(send_data + ((i * count) * type_size), count, MPI_INT, i, 123, MPI_COMM_WORLD);
        }
    } else MPI_Recv(recv_data, count, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

int main(int argc, char** argv) {
    int rank, size, i;
    const int   ELEM_PER_PROCESS = 125; 
    double start_t, elapsed_t;
    double glob_elapsed;
     
    MPI_Init(&argc, &argv);
    get_device_info(&rank, &size);    
    int len 	= size * ELEM_PER_PROCESS;

    int *collective_array = NULL;
    if (rank == 0)
    {   
        collective_array = (int*)malloc(sizeof(int) * len);           
        for(i=0; i<len; i++) collective_array[i] = 1;
    }
    
    //allocate small arrays for each process
    int* sub_array = (int*)malloc(sizeof(int)*ELEM_PER_PROCESS);
    assert(sub_array != NULL);    
     
    //make all processes wait for each other
    MPI_Barrier(MPI_COMM_WORLD);
    start_t = MPI_Wtime();

    //custom scatter:
    Scatter(collective_array, sub_array, ELEM_PER_PROCESS);     
    
    MPI_Gather(sub_array, ELEM_PER_PROCESS, MPI_INT, collective_array, ELEM_PER_PROCESS, MPI_INT, 0, MPI_COMM_WORLD);
    elapsed_t = MPI_Wtime() - start_t;

    //now wait for all ranks and check timing:
    MPI_Barrier(MPI_COMM_WORLD);
    glob_elapsed = MPI_Wtime() - start_t;

    printf("%g in rank: %d.\n", elapsed_t, rank);
    fflush(stdout);
    
    if(rank == 0)
    {
        printf("\n---------------------------\n%g globally.\n---------------------------\n", glob_elapsed);
        fflush(stdout);
        free(collective_array);
       
    }  
   free(sub_array);
    MPI_Finalize();
    return 0;
} 
