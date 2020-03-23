#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define SEED time(NULL)

int main(int argc, char* argv[]) {
 //-------------------------------
 //inicjalizacja
 int rank, size;
 double sendbuf, recvbuf;
 double start_t, end_t;
 
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 
 srand( SEED );
 long long int i, count, n;
 double x,y,z,pi;
 char* eptr;
 n = strtol(argv[1], &eptr, 10) / size; //liczba losowań w każdym procesie
  
 //-------------------------------
 //bariera i  pomiar czasu
 MPI_Barrier(MPI_COMM_WORLD);
 start_t = MPI_Wtime();
 
 //-------------------------------
 //obliczenia
 count = 0;   
 for(i = 0; i < n; ++i) {
     x = (double)rand() / RAND_MAX;
     y = (double)rand() / RAND_MAX;
     z = x * x + y * y;

     if( z <= 1 ) count++;
 }
 sendbuf = (double) count / n * 4;
 
 //---------------------------------
 //komunikacja między wątkami i pomiar czasu
 MPI_Reduce(&sendbuf, &recvbuf, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
 end_t = MPI_Wtime() - start_t;

 //--------------------------------
 //wypis
 if(rank == 0)
 {
    pi = recvbuf / size;
    printf("Executed on %d processes.\nApproximate PI = %g.\nExecution time: %g.\n.\n", size, pi, end_t);
 }
 
 //--------------------------------
 //finalizacja
 MPI_Finalize();
 return(0);
}
 
