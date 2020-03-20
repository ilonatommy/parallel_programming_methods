#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define SEED time(NULL)

int main(int argc, char* argv[]) {
 int rank, size;
 //pozwala na trzymanie sumarycznie aż do 18,446,744,073,709,551,61 punktów, które znajdziemy w środku koła
 //innym sposobem byloby liczenie pi osobno w każdym procesie i usrednianie wyniku
 double sendbuf_1, recvbuf_1, sendbuf_2, recvbuf_2;
 double start_t, end_t;
 
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 
 srand( SEED );
 long long int i, count, n;
 double x,y,z,pi;
 char* eptr;
 
 if(rank==0) printf("You passed: %s as a number of points.\n", argv[1]);
 n = strtol(argv[1], &eptr, 10) / size; //liczba losowań w każdym procesie
 
 //petla usredniajaca: 100-krotna
 int iterations, j;
 double* all_pi;
 double sd, summed_pi;
 iterations = 10;
 summed_pi = 0;
 sd = 0;
 all_pi = (double*)malloc(iterations*sizeof(double));
 
 MPI_Barrier(MPI_COMM_WORLD);
 start_t = MPI_Wtime();
 
 for(j=0; j<iterations; j++){
 count = 0; //liczba punktów znalezionych w kole
  
  //losowanie n punktów:
  for(i = 0; i < n; ++i) {
      x = (double)rand() / RAND_MAX;
      y = (double)rand() / RAND_MAX;
      z = x * x + y * y;

      if( z <= 1 ) count++;
  }
  all_pi[j] = (double) count / n * 4;
  summed_pi += all_pi[j];  
 }
 
 //średnie pi obliczone przez jeden proces:  
 pi = summed_pi / iterations;
 sendbuf_1 = pi;
 
 //zsumuj wszystkie średnie pi z procesów i zapisz do recvbuf
 MPI_Reduce(&sendbuf_1, &recvbuf_1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

 //odchylenie:
 for(j=0; j<iterations; j++)
  sd += pow(all_pi[j] - pi, 2); 
 sendbuf_2 = sqrt(sd/iterations);
 
 //zsumuj wszystkie odchylenia, bo będziemy je uśredniać w procesie głównym
 MPI_Reduce(&sendbuf_2, &recvbuf_2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
 
 //tu nie trzeba dodawać bariery, bo MPI_Reduce() działa też jak bariera 
 end_t = MPI_Wtime() - start_t;

 //tylko proces główny oblicza ostateczny wynik 
 //to jest dość szybkie, więc nie musimy brać pod uwagę w czasie pracy programu
 if(rank == 0)
 {
    pi = recvbuf_1 / size;
    sd = recvbuf_2 / size;
    printf("Executed on %d processes.\nApproximate PI = %g.\nExecution time: %g.\nSD = %g.\n", size, pi, end_t, sd);
 }
 
 MPI_Finalize();
 return(0);
}
 
