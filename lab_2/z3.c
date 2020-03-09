#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//https://stackoverflow.com/questions/52797233/problems-implementing-mpi-scatter-with-mpi-send-and-mpi-recv?rq=1

void Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, 
                  void* recv_data, int recv_count, MPI_Datatype recv_datatype, 
                  int root, MPI_Comm communicator) {

    int np, rank;
    int i;
    int size;

    MPI_Datatype type;
    type = send_datatype;
    MPI_Type_size(type, &size);

    MPI_Status status;
    MPI_Comm_size(communicator, &np);
    MPI_Comm_rank(communicator, &rank);



    if (rank == 0) {
        for (i = 0; i < np; i++) {
            MPI_Send(send_data + ((i * send_count) * size), send_count, send_datatype, i, 0, communicator);

        }
    }
    MPI_Recv(recv_data, recv_count, recv_datatype, root, 0, communicator, &status);

}

void get_device_info(int* rank, int*size)
{
    MPI_Comm_rank (MPI_COMM_WORLD, rank);  /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, size);  /* get number of processes */
    //printf("Current process id %d, number of processes %d.\n", *rank, *size);
}

int main(int argc, char** argv) {
    int rank, size, i;
    const int   ELEM_PER_PROCESS = 1250000; 
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
    int sub_array[ELEM_PER_PROCESS];
    assert(sub_array != NULL);    
     
    //make all processes wait for each other
    MPI_Barrier(MPI_COMM_WORLD);
    start_t = MPI_Wtime();

    //send the table of ELEM_PER_PROCESS elements to each of the processes
    //MPI_Scatter(collective_array, ELEM_PER_PROCESS, MPI_INT, sub_array, ELEM_PER_PROCESS, MPI_INT, 0, MPI_COMM_WORLD);
    //custom scatter:
    Scatter(collective_array, ELEM_PER_PROCESS, MPI_INT, sub_array, ELEM_PER_PROCESS, MPI_INT, 0, MPI_COMM_WORLD);    

    //do the computations for each rank in parallel
    for(i=0; i<ELEM_PER_PROCESS; i++) sub_array[i] *= rank;
    
    MPI_Gather(sub_array, ELEM_PER_PROCESS, MPI_INT, collective_array, ELEM_PER_PROCESS, MPI_INT, 0, MPI_COMM_WORLD);
    elapsed_t = MPI_Wtime() - start_t;

    //now wait for all ranks and check timing:
    MPI_Barrier(MPI_COMM_WORLD);
    glob_elapsed = MPI_Wtime() - start_t;

    printf("%g in rank: %d.\n", elapsed_t, rank);
    fflush(stdout);
    
    if(rank == 0)
    {
	for(i=0; i<len; i++) printf("%d, ", collective_array[i]);
        //printf("\n---------------------------\n%g globally.\n---------------------------\n", glob_elapsed);
        free(collective_array);
    }  
   
    MPI_Finalize();
    return 0;
} 
