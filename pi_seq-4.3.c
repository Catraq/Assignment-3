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

	if(rank == 0)
	{
		
		double *pi_global = (double *)malloc((size - 1) * sizeof(double)); 
		MPI_Request *request = (MPI_Request *)malloc((size -1) * sizeof(MPI_Request)); 
	
		double start_time = MPI_Wtime();
		
		
		for(int i = 1; i < size; i++)
		{
			MPI_Irecv(&pi_global[i-1], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &request[i-1]);
		}	
		
		MPI_Waitall(size-1, request, MPI_STATUSES_IGNORE);
		
		float pi = 0;
		for(int i = 0; i < size-1; i++)
		{
			pi += pi_global[i];
		}
		pi = pi/(size-1);
		
		double stop_time = MPI_Wtime();
		double elapsed_time = stop_time - start_time;
		
		printf("time: %f \n", elapsed_time);
		printf("The result is %f\n", pi);
		
		
		
		free(request);
		free(pi_global);
	}
	else
	{
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
	
		MPI_Request transfered; 
		MPI_Isend(&pi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &transfered);
		MPI_Wait(&transfered, MPI_STATUS_IGNORE);
	}
	


	MPI_Finalize();

	return 0;
}

