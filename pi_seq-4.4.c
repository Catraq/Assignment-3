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

	double pi_sum = 0.0;
	MPI_Reduce(&pi, &pi_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if(rank == 0)
	{
		printf("pi=%f \n", pi_sum/size);
		double stop_time = MPI_Wtime();
		double elapsed_time = stop_time - start_time;
		
		printf("time: %f \n", elapsed_time);
	}
	


	MPI_Finalize();

	return 0;
}

