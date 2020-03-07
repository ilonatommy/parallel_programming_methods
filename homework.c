 
#include <stdio.h>
#include <mpi.h>

void do_MPI_Send(int from_proc, int to_proc, int* buf)
{
    *buf = from_proc * 111;
    MPI_Send(buf, 1, MPI_INT, to_proc, 123, MPI_COMM_WORLD);
    printf("Process %d MPI_Send buffer %d to %d.\n", from_proc, *buf, to_proc);
}

void do_MPI_Recv(int from_proc, int to_proc, int* buf, MPI_Status status)
{
    int expected_buf = from_proc * 111;
    MPI_Recv(buf, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
    if (*buf != expected_buf)
        printf("Error: buffer = %d but is expected to be %d\n", *buf, expected_buf);
    else printf("Process %d MPI_Recv buffer %d from process %d.\n", to_proc, *buf, from_proc);
    fflush(stdout);
}

int main (int argc, char * argv[])
{
    int rank, size;
    int buf = -1;
    
     MPI_Status status;

    MPI_Init (&argc, &argv);  /* starts MPI */
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);  /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, &size);  /* get number of processes */
  
    if(size < 2)
    {
        printf("Please run with two processes.\n");
        fflush(stdout);
        MPI_Finalize();
        return 0;
    }
    
    if (rank == 0)
    {
        int from_proc = rank;
        int to_proc = rank + 1;
        buf = 0;
        MPI_Send(&buf, 1, MPI_INT, to_proc, 123, MPI_COMM_WORLD);
        printf("Process %d MPI_Send buffer %d to %d.\n", from_proc, buf, to_proc);
    }
    
    if (rank == 1)
    {
        int from_proc = rank - 1;
        int to_proc = rank;
        int expected_buf = from_proc * 111;
	buf = -1;
        MPI_Recv(&buf, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
        if (buf != expected_buf)
            printf("Error: buffer = %d but is expected to be %d\n", buf, expected_buf);
        else printf("Process %d MPI_Recv buffer %d from process %d.\n", to_proc, buf, from_proc);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}
