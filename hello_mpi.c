#include <mpi.h>
#include <stdio.h>

int main(int args, char *argv[])
{
	int dummy; 
	MPI_Init_thread(&args, &argv, MPI_THREAD_SINGLE, &dummy);
	
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("Hello world from %i of %i \n", rank, size);

	MPI_Finalize();

	return 0;
}
