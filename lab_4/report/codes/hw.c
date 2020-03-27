#include <stdio.h>
#include "mpi.h"

void get_device_info(int* rank, int*size)
{
    MPI_Comm_rank (MPI_COMM_WORLD, rank);  /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, size);  /* get number of processes */
    //printf("Current process id %d, number of processes %d.\n", *rank, *size);
}

int main(int argc, char** argv) {
    int rank, size, i;
    int         n = 10000;
    MPI_Status  status; 
    double      start, elapsed;
   
    MPI_Init(&argc, &argv);
    get_device_info(&rank, &size);    
    
    int len = 2<<13; //
    
    /* opoznienie */
    if (rank == 0) 
    {   
           int k[len];
           start = MPI_Wtime();
           for (i = 0; i < n; ++i)
           {   
               k[0] = i;
               MPI_Send(k, len, MPI_INT, 1, 123, MPI_COMM_WORLD);
               MPI_Recv(k, len, MPI_INT, 1, 123, MPI_COMM_WORLD, &status);
               if (k[0] != i+1) printf("error\n");
           }   
         elapsed = MPI_Wtime() - start;
         printf("av message of length %d has time of %g sec\n", len,  elapsed/(2*n)); 		 
         fflush(stdout);
       
    }

    /* przepustowosc */
    if (rank == 1)
    { 
           int k[len];
           start = MPI_Wtime();
           for (i = 0; i < n; ++i)
    	   {  k[0] = i;
              MPI_Send(k, len, MPI_INT, 0, 123, MPI_COMM_WORLD);
              MPI_Recv(k, len, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
       	      if (k[0] != i+1) printf("error\n");
           }
           elapsed = MPI_Wtime() - start;
           long int s = sizeof(k);
           printf("av message of size %lu has %g Mbit/s\n", 
                  8*s, 8*s/ (1000000 * elapsed/(2*n)));
           fflush(stdout);
       }
     MPI_Finalize();
     return 0;
} 
