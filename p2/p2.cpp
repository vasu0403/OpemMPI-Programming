#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

const int root_rank = 0;

const int MAX_INPUT_SIZE = 1000001;
int sorted_segments[MAX_INPUT_SIZE], sorted_array[MAX_INPUT_SIZE], unsorted_array[MAX_INPUT_SIZE];
int partition(int arr[], int l, int r) {
	int size = r - l + 1;
	int random_index = (rand() % size + l);
	swap(arr[l], arr[random_index]);
    int pivot = arr[l];
    int pivot_index = l;
    int i = l + 1, j = r;
    while(i <= j) {
        if(arr[i] <= pivot) {
            i++;
            pivot_index++;
        } else {
            swap(arr[i], arr[j]);
            j--;
        }
    }
    swap(arr[l], arr[pivot_index]);
    return pivot_index;
}
void quick_sort(int arr[], int l, int r) {
    if(l < r) {
        int p = partition(arr, l, r);
        quick_sort(arr, l, p - 1);
        quick_sort(arr, p + 1, r);
    }
};
void k_way_merge(int left_index[], int right_index[], int numprocs, int N, char *file) {
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    freopen(file, "w", stdout);
	for(int i = 0; i < numprocs; i++) {
		if(left_index[i] <= right_index[i] != 0) {
			pq.push({sorted_segments[left_index[i]], i});
			left_index[i]++;
		}
	}

	int done = 0;
	while(!pq.empty()) {
		pair<int, int> smallest = pq.top();
		pq.pop();
		sorted_array[done++] = smallest.first;
		int P = smallest.second;
		if(left_index[P] <= right_index[P]) {
			pq.push({sorted_segments[left_index[P]], smallest.second});
			left_index[smallest.second]++;
		}
	}
	fstream out_file;
	for(int i = 0; i < N; i++) {
		printf("%d ", sorted_array[i]);
	}
    printf("\n");

}
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

    int N;
    if(rank == root_rank) {
        /* root process. Read the input. */
        freopen(argv[1], "r", stdin);
        scanf("%d", &N);
        for(int i = 0; i < N; i++) {
            scanf("%d", &unsorted_array[i]);
        }
        /* broadcast the size of entire array to each process. */
        MPI_Bcast(&N, 1, MPI_INT, root_rank, MPI_COMM_WORLD);

        int per_process = N / numprocs;
        int left_over = N % numprocs;
        int partial_size = per_process + (root_rank < left_over ? 1 : 0);

        /* send the partial arrays to each process */
        int cur_pos = partial_size;
        for(int i = 1; i < numprocs; i++) {
            int cur_process_size = per_process + (i < left_over ? 1: 0);
            MPI_Send(unsorted_array + cur_pos, cur_process_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            cur_pos += cur_process_size;
        }

        int partial_array[partial_size];
        for(int i = 0; i < partial_size; i++) {
            partial_array[i] = unsorted_array[i];
        }

        quick_sort(partial_array, 0, partial_size - 1);

        int left_index[numprocs], right_index[numprocs];
        for(int i = 0; i < partial_size; i++) {
        	sorted_segments[i] = partial_array[i];
        }
        left_index[0] = 0;
        right_index[0] = partial_size - 1;
        cur_pos = partial_size;
        for(int i = 1; i < numprocs; i++) {
        	int cur_partial_size = per_process + (i < left_over ? 1 : 0);
        	MPI_Recv(sorted_segments + cur_pos, cur_partial_size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        	left_index[i] = cur_pos;
        	right_index[i] = cur_pos + cur_partial_size - 1;
        	cur_pos += cur_partial_size;
        }

        k_way_merge(left_index, right_index, numprocs, N, argv[2]);    

    } else {
        MPI_Bcast(&N, 1, MPI_INT, root_rank, MPI_COMM_WORLD);

        /* Calculate the size of the partial array which the root process will pass onto the current process */
        int per_process = N / numprocs;
        int left_over = N % numprocs;
        int partial_size = per_process + (rank < left_over ? 1 : 0);
        int partial_array[partial_size];

        /* receive the partial array from the root process */
        MPI_Recv(partial_array, partial_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        quick_sort(partial_array, 0, partial_size - 1);

        MPI_Send(partial_array, partial_size, MPI_INT, root_rank, 0, MPI_COMM_WORLD);   
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