#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void rand_fill_array(int* array, long chunk_size, long long len)
{
     int i;
     unsigned int id;
#pragma omp parallel default(none) private(i, id) shared(array, len, chunk_size)
     {
        id = omp_get_thread_num();
        srand(time(0)*(1+id));
#pragma omp for schedule(guided) private(id)
         for (i = 0; i < len; ++i)
         {
             id = omp_get_thread_num();
             array[i] = rand_r(&id);
         }
     }
}

int main(int argc, char* argv[])
{
    int num_of_threads = atoi(argv[1]);
    omp_set_num_threads(num_of_threads);

    long long len, chunk_size;
    int *source_array;
    float t, begin_t, end_t;

    len = 250000000;
    chunk_size = (int)(len / num_of_threads);
    source_array = (int*)malloc(len * sizeof(int));
    
    begin_t = omp_get_wtime();
    rand_fill_array(source_array, chunk_size, len);   
    end_t = omp_get_wtime();
    t = end_t - begin_t;
    printf("%f\n", t);

    free(source_array);
    return 0;
}

