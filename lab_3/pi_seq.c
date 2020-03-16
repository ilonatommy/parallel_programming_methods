 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define SEED time(NULL)

int main(int argc, char* argv[]) {

 srand( SEED );
 long int n, i, count;
 double x,y,z,pi;
 char* eptr;
 
 printf("You passed: %s as a number of points.\n", argv[1]);
 //ilosc punktow podajemy jako argument
 n =  strtol(argv[1], &eptr, 10);

 //petla usredniajaca: 100-krotna
 int iterations, j;
 double* all_pi;
 double sd, summed_pi;
 iterations = 100;
 summed_pi = 0;
 sd = 0;
 all_pi = (double*)malloc(iterations*sizeof(double));

 for(j=0; j<iterations; j++){
  count = 0;

  for(i = 0; i < n; ++i) {
      //losowanie niezależne:
      x = (double)rand() / RAND_MAX;
      y = (double)rand() / RAND_MAX;
     
      //liczenie długości wektora:
      z = x * x + y * y;
     
      //sprawdzenie czy jest w kole:
      if( z <= 1 ) count++;
  }
  all_pi[j] = (double) count / n * 4;
  summed_pi += all_pi[j];
 }
 
 //średnia:  
 pi = summed_pi / iterations;
 printf("PI = %g\n", pi);
 
 //odchylenie:
 for(j=0; j<iterations; j++)
  sd += pow(all_pi[j] - pi, 2); 
 sd = sqrt(sd/iterations);
 printf("SD = %g\n", sd);
 return(0);
}
