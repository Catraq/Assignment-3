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
	
	int modulus = 1;
	do
	{	
		//printf("rank=%i, mod=%i \n", rank, modulus);
		
		//If this rank won't be in next level. Send value up one level. 
		if(rank%(modulus*2) != 0){
			//printf("%i -> %i \n", rank, rank-modulus);
			MPI_Send(&pi, 1, MPI_DOUBLE, rank-modulus, 0, MPI_COMM_WORLD);
		}
		//If the rank "above" this was in previous but not this level. 
		if((rank + modulus/2)%modulus!= 0){
			double recv_pi = 0;
			MPI_Recv(&recv_pi, 1, MPI_DOUBLE, rank+modulus/2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//printf("%i <- %i \n", rank, rank+modulus/2);
			
			pi = (pi + recv_pi)/2.0;
		}
		
		
		
		if(modulus == size)
			break;
		
		modulus = modulus*2;
		
	}
	while(rank%modulus  == 0);
	
	double stop_time = MPI_Wtime();
	double elapsed_time = stop_time - start_time;
	if(rank == 0)
	{
		printf("time: %f \n", elapsed_time);
	}
	
	if(rank == 0)
		printf("pi=%f \n", pi);
		
	MPI_Finalize();

	return 0;
}

