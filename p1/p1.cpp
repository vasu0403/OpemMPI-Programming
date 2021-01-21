#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

const int root_rank = 0;
int main( int argc, char **argv ) {
    int rank, numprocs;
    double estimated_value;

    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();
    MPI_Status status;

    int N;
    if(rank == root_rank) {
        /* 
            This is the root process 
            Read input and distribute it to all the processes.
        */
        fstream input_file, output_file;
        input_file.open("in.txt", ios::in);

        input_file >> N;
        input_file.close();
    }

    MPI_Bcast(&N,1,MPI_INT,root_rank,MPI_COMM_WORLD);
    int per_process = N / numprocs;
    int left_over = N % numprocs;
    
    int num = per_process + (rank < left_over ? 1 : 0);
    int start = rank*per_process + min(left_over, rank) + 1;
    double partial_value = 0;
    for(int i = start; i < start + num; i++) {
        partial_value += (1.0 / (i * i));
    }
    
    MPI_Reduce(&partial_value, &estimated_value, 1, MPI_DOUBLE, MPI_SUM, root_rank, MPI_COMM_WORLD);
    if(rank == root_rank) {
        cout << fixed << setprecision(6) << estimated_value << "\n";
    }
    
    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf( "Total time (s): %f\n", maxTime );
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}