#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
	int count = 0;
	double x, y, z, pi;

	MPI_Init(&argc, &argv);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(rank); // Important: Multiply SEED by "rank" when you introduce MPI!

	MPI_Barrier(MPI_COMM_WORLD);
	double start_time = MPI_Wtime();


	// Calculate PI following a Monte Carlo method
	for (int iter = 0; iter < NUM_ITER/size; iter++)
	{
		// Generate random (X,Y) points
		x = (double)random() / (double)RAND_MAX;
		y = (double)random() / (double)RAND_MAX;
		z = sqrt((x*x) + (y*y));

		// Check if point is in unit circle
		if (z <= 1.0)
		{
			count++;
		}
	}

	// Estimate Pi and display the result
	pi = ((double)count / (double)(NUM_ITER/size)) * 4.0;
	
	MPI_Barrier(MPI_COMM_WORLD);
	double stop_time = MPI_Wtime();
	double elapsed_time = stop_time - start_time;

	if(rank == 0)
	{
		printf("time: %f \n", elapsed_time);
	}


	if(rank == 0){
		double *rank_avg = (double *)malloc(size * sizeof(double));
		rank_avg[0] = pi;
		pi = 0;	

		for(int i = 1; i < size; i++)
		{
			MPI_Recv(&rank_avg[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}			

		for(int i = 0; i < size; i++)
		{
			pi += rank_avg[i];
		}
		free(rank_avg);

		pi = pi/size;

		printf("The result is %f\n", pi);


	}else
	{
		MPI_Send(&pi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);	
	}


	MPI_Finalize();

	return 0;
}

