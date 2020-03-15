#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
 
/* A simple test of Reduce with all choices of root process */
int main( int argc, char *argv[] )
{
    int errs = 0;
    int rank, size;
    int *sendbuf, *recvbuf, i;
    int count;
    MPI_Comm comm;
 
    MPI_Init( &argc, &argv );
 
    comm = MPI_COMM_WORLD;
    /* Determine the sender and receiver */
    MPI_Comm_rank( comm, &rank );
    MPI_Comm_size( comm, &size );
   
    //szybkie how to: każdy proces dostaje bufor z jedynkami, 
    //redukcja zbiera elementy o tym samym indeksie i cos z nimi robi: w tym wypadku sumuje
    //w ten sposob powstaje bufor receive znajdujacy sie w roocie(0)
    count = 2;
    sendbuf = (int *)malloc( count * sizeof(int) );
    recvbuf = (int *)malloc( count * sizeof(int) );
    for (i=0; i<count; i++) sendbuf[i] = 1;
    for (i=0; i<count; i++) recvbuf[i] = -1;
    MPI_Reduce( sendbuf, recvbuf, count, MPI_INT, MPI_SUM, 0, comm );
    if (rank == 0) {
       for (i=0; i<count; i++) {
            if (recvbuf[i] != 1*size) {
                errs++;
             }
	     printf("%d,", recvbuf[i]);
        }
        printf("Error count: %d.", errs);
    }     
    free( sendbuf );
    free( recvbuf );
 
    MPI_Finalize();
    return 0;
}
