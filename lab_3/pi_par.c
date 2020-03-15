#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define SEED time(NULL)

int main() {
 int rank, size, av_points,
 //pozwala na trzymanie sumarycznie aż do 18,446,744,073,709,551,61 punktów, które znajdziemy w środku koła
 //innym sposobem byloby liczenie pi osobno w każdym procesie i usrednianie wyniku
 unsigned long long int *sendbuf, *recvbuf;
 MPI_Comm comm;
 
 srand( SEED );
 int i, count, n;
 double x,y,z,pi;
 
 n = 100; //liczba losowań w każdym procesie
 count = 0; //liczba punktów znalezionych w kole

 //losowanie w każdym procesie:
 for(i = 0; i < n; ++i) {
     x = (double)rand() / RAND_MAX;
     y = (double)rand() / RAND_MAX;
     z = x * x + y * y;

     if( z <= 1 ) count++;
 }
 *sendbuf = count;

 //sprawdzanie liczby PI tylko w jednym procesie
 if(rank == 0)
 {
    //zbierz wszystkie punkty wewnątrz koła od procesów (sumując)
    MPI_Reduce(sendbuf, recvbuf, n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    av_points = (*recvbuf) / size;
    pi = (double) av_points / n * 4; 
    printf("Approximate PI = %g", pi);
 }
 
free( sendbuf );
free( recvbuf );
 
MPI_Finalize();
return(0);
}
 
