#include <time.h> 
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdbool.h>

void do_MPI_Send(int from_proc, int to_proc, int* buf, int buf_size, int value, bool verbose)
{
    int i;
    for(i=0; i<buf_size; i++)
      buf[i] = value;
    MPI_Send(buf, buf_size, MPI_INT, to_proc, 123, MPI_COMM_WORLD);
    if(verbose)  printf("Process %d MPI_Send buffer to %d.\n", from_proc, to_proc);
}

void do_MPI_Recv(int from_proc, int to_proc, int* buf, int buf_size, MPI_Status* status, int value, bool verbose)
{
    if(verbose)
    {
        int i = 0;
        for(i=0; i<buf_size; i++)
            buf[i] = -1;
    }
    int expected_buf = value;
    MPI_Recv(buf, buf_size, MPI_INT, 0, 123, MPI_COMM_WORLD, status);
    for(i=0; i<buf_size; i++)
        if (buf[i] != expected_buf)
        {
           printf("Error: buffer[%d] = %d but is expected to be %d\n", i, buf[i], expected_buf);
           fflush(stdout);
           return;
        }
    if(verbose) printf("Process %d MPI_Recv buffer of %d elements from process %d.\n", to_proc, buf_size, from_proc);
    fflush(stdout);
}

void get_device_info(int* rank, int*size)
{
    MPI_Comm_rank (MPI_COMM_WORLD, rank);  /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, size);  /* get number of processes */
    //printf("Current process id %d, number of processes %d.\n", *rank, *size);
}

int main (int argc, char * argv[])
{
    int rank, size, buf_size;
    int* buf = malloc(sizeof(int));
    
    MPI_Status status;
    MPI_Init (&argc, &argv);  /* starts MPI */
    get_device_info(&rank, &size);

    if(size < 2)
    {
        printf("Please run with two processes.\n");
        fflush(stdout);
        MPI_Finalize();
        return 0;
    }
    int pow = 0;
    int value = 0;

    //create timer
    clock_t t;
    double total_t = 0;
    int max_iter = 500;
    for(pow=10; pow < 30; pow+=2)
    {
        buf_size = 2<<pow;
        buf = realloc(buf, sizeof(int) * buf_size);
        int test_i;
        
        //start measuring time (in seconds, based on the buffor size):
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) 
        {
            start_time = MPI_Wtime();
            for (i = 0; i < max_iter; ++i)
            {
                do_MPI_Send(rank, rank+1, buf, buf_size, value, false);
                do_MPI_Recv(rank, rank+1, buf, buf_size, &status, value, false);
            }            
            elapsed_time = MPI_Wtime() - start_time;
            printf("%d ping-pongs in %g seconds, ", max_iter, elapsed_time);
            printf(" for an average message time of %g\n", elapsed_time/(2*max_iter));
        }
        
        //start measuring throughput
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 1) 
        {   
            start_time = MPI_Wtime();
            for (i = 0; i < n; ++i)
            {   
                do_MPI_Send(rank-1, rank, buf, buf_size, value, false);
                do_MPI_Recv(rank-1, rank, buf, buf_size, &status, value, false);
            }
            elapsed_time = MPI_Wtime() - start_time;
            printf("%d ping-pongs in %g seconds, ", n, elapsed_time);
            printf(" for an average message time of %g\n", (sizeof(int) * buf_size)/ (elapsed_time/(2*max_iter)));
        }
        value++;    
    }
    MPI_Finalize();
    return 0;
}
