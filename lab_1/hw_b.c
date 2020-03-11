#include <stdio.h>
#include "mpi.h"

void get_device_info(int* rank, int*size)
{
    MPI_Comm_rank (MPI_COMM_WORLD, rank);  /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, size);  /* get number of processes */
    //printf("Current process id %d, number of processes %d.\n", *rank, *size);
}

int main(int argc, char** argv) {
    int rank, size;
    int         tag = 123;
    int         n = 10000;   /* number of ping-pong's back and forth */
    int         i;
    MPI_Status  status;
    MPI_Request req1, req2; 
    double      start_time, elapsed_time;
   
    MPI_Init(&argc, &argv);
    get_device_info(&rank, &size);    
    
    int len = 2<<14;
    // measure delay
    if (rank == 0) /* Note:  it is not always valid that process one can output */
    {   
           int k[len];
           start_time = MPI_Wtime();
           for (i = 0; i < n; ++i)
           {   
               k[0] = i;
               MPI_Isend(k, len, MPI_INT, 1, tag, MPI_COMM_WORLD,&req1);
               MPI_Irecv(k, len, MPI_INT, 1, tag, MPI_COMM_WORLD, &req2);
               MPI_Wait(&req1, &status);
               MPI_Wait(&req2, &status);
               if (k[0] != i) printf("invalid pong received\n");
           }   
  	 elapsed_time = MPI_Wtime() - start_time;
         printf("for an average messageof size %lu bandwidth of %g Mbit/s\n",8* sizeof(k),8* sizeof(k)/ (1000000 * elapsed_time/(2*n))); 		 
         fflush(stdout);
       
    }
    // measure bandwidth
    if (rank == 1)
    { 
           int k[len];
           start_time = MPI_Wtime();
           for (i = 0; i < n; ++i)
    	   {  k[0] = i;
              MPI_Isend(k, len, MPI_INT, 0, tag, MPI_COMM_WORLD, &req1);
              MPI_Irecv(k, len, MPI_INT, 0, tag, MPI_COMM_WORLD, &req2);
              MPI_Wait(&req1, &status);
              MPI_Wait(&req2, &status);
       	      if (k[0] != i) printf("invalid pong received\n");
           }
           elapsed_time = MPI_Wtime() - start_time;
           printf("for an average message of length %d time of %g sec\n", len,  elapsed_time/(2*n));
           fflush(stdout);
     }
     MPI_Finalize();
     return 0;
} 
