
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

int main(int argc, char *argv[]){

    int rank, size, i, provided;
    
    // number of cells (global)
    int nxc = 128; // make sure nxc is divisible by size
    double L = 2*3.1415; // Length of the domain
    

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // number of nodes (local to the process): 0 and nxn_loc-1 are ghost cells 
    int nxn_loc = nxc/size + 3; // number of nodes is number cells + 1; we add also 2 ghost cells
    double L_loc = L/((double) size);
    double dx = L / ((double) nxc);
    
    // define out function
    double *f = calloc(nxn_loc, sizeof(double)); // allocate and fill with z
    double *dfdx = calloc(nxn_loc, sizeof(double)); // allocate and fill with z

    for (i=1; i<(nxn_loc-1); i++)
      f[i] = sin(L_loc*rank + (i-1) * dx);
    
    
    uint32_t next_rank = (rank+1)%size;
    uint32_t prev_rank = ((uint32_t)(rank+size-1))%size;

#if 0
    f[0] = f[nxn_loc-3];
    f[nxn_loc-1] = f[2];
#endif 
    
#if 1
    
    MPI_Send(&f[nxn_loc-3], 1, MPI_DOUBLE, (int)next_rank, 0, MPI_COMM_WORLD);
    MPI_Recv(&f[0], 1, MPI_DOUBLE, (int)prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

       
    MPI_Send(&f[2], 1, MPI_DOUBLE, (int)prev_rank, 0, MPI_COMM_WORLD); 
    MPI_Recv(&f[nxn_loc-1], 1, MPI_DOUBLE, (int)next_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   
	
#endif

    // here we finish the calculations
   #if 1
    // calculate first order derivative using central difference
    // here we need to correct value of the ghost cells!
    for (i=1; i<(nxn_loc-1); i++)
      dfdx[i] = (f[i+1] - f[i-1])/(2*dx);

    for (i=1; i<(nxn_loc-1); i++){
    	float delta = cos(L_loc*rank + (i-1) * dx) - dfdx[i];
    	if(!(delta < 0.01 && delta > -0.01))
    	{
    		printf("Error \n");
    	}
    }
    printf("\n");
 
    // Print f values
    if (rank==0){ // print only rank 0 for convenience
        printf("My rank %d of %d\n", rank, size );
        printf("Here are my values for f including ghost cells\n");
        
        for (i=0; i<nxn_loc; i++)
	       printf("%f\n", f[i]);
        printf("\n"); 
    	
    }   
    #endif
    
    

    MPI_Finalize();
}






