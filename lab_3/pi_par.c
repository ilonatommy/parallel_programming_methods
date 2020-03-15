#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define SEED time(NULL)

int main(int argc, char* argv[]) {
 int rank, size, av_points;
 //pozwala na trzymanie sumarycznie aż do 18,446,744,073,709,551,61 punktów, które znajdziemy w środku koła
 //innym sposobem byloby liczenie pi osobno w każdym procesie i usrednianie wyniku
 long int sendbuf, recvbuf;
 double start_t, end_t;
 
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 
 MPI_Barrier(MPI_COMM_WORLD);
 start_t = MPI_Wtime();

 srand( SEED );
 int i, count, n;
 double x,y,z,pi;
 
 n = 2000000; //liczba losowań w każdym procesie
 count = 0; //liczba punktów znalezionych w kole
 
 //losowanie w każdym procesie:
 for(i = 0; i < n; ++i) {
     x = (double)rand() / RAND_MAX;
     y = (double)rand() / RAND_MAX;
     z = x * x + y * y;

     if( z <= 1 ) count++;
 }
 sendbuf = count;
 MPI_Reduce(&sendbuf, &recvbuf, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

 MPI_Barrier(MPI_COMM_WORLD);
 end_t = MPI_Wtime() - start_t;

 //sprawdzanie liczby PI tylko w jednym procesie
 if(rank == 0)
 {
    av_points = (recvbuf) / size;
    pi = (double) av_points / n * 4; 
    printf("Approximate PI = %g.\n", pi);
    printf("Execution time: %g\n.", end_t);
 }
 
 MPI_Finalize();
 return(0);
}
 
