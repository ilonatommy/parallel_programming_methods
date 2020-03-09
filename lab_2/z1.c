#include <stdio.h>
#include "mpi.h"

void get_device_info(int* rank, int*size)
{
    MPI_Comm_rank (MPI_COMM_WORLD, rank);  /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, size);  /* get number of processes */
    //printf("Current process id %d, number of processes %d.\n", *rank, *size);
}

int main(int argc, char** argv) {
    int rank, processes_num, i;
    const int   ELEM_PER_PROCESS = 3;
    int         len = size*ELEM_PER_PROCESS;
    int         tag = 123;
    MPI_Status  status; 
    double      start_time, elapsed_time;
   
    MPI_Init(&argc, &argv);
    get_device_info(&rank, &size);    
    
    int len = 2<<13;
    // measure delay
    if (rank == 0) /* Note:  it is not always valid that process one can output */
    {   
        int collective_array[len];           
        for(i=0; i<len; i++) collective_array[i] = 1;
    }
    
    //allocate small arrays for each process
    int sub_array[ELEM_PER_PROCESS];
    assert(sub_array != NULL);
    
    //send the table of ELEM_PER_PROCESS elements to each of the processes
    MPI_Scatter(collective_array, ELEM_PER_PROCESS, MPI_INT, sub_array, ELEM_PER_PROCESS, MPI_INT, 0, MPI_COMM_WORLD);
    
    //do the computations for each rank in parallel
    for(i=0; i<ELEM_PER_PROCESS; i++) sub_array[i] *= rank;
    
    MPI_Gather(sub_array, ELEM_PER_PROCESS, MPI_INT, collective_array, ELEM_PER_PROCESS, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank == 0)
    {
        printf"[");
        for(i=0; i<len; i++) printf("%d, ", collective_array[i]);
        printf"]");
        
        free(sub_array);
        free(collective_array);
    }  
   
    MPI_Finalize();
    return 0;
} 
