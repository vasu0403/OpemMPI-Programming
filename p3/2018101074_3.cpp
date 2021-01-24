/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

const int root_rank = 0;
int adj[501][501];
int available_colors[501][501], color[501];
int main( int argc, char **argv ) {
    int rank, numprocs;

    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();
    int N, M;
    if(rank == 0) {
        fstream in_file;
        in_file.open(argv[1], ios::in);

        in_file >> N >> M;
        vector<pair<ll, ll>> edges(M);
        for(int i = 0, u, v; i < M; i++) {
            in_file >> u >> v;
            edges[i].first = u;
            edges[i].second = v;
        }
        for(int i = 0; i < M; i++) {
            for(int j = i + 1; j < M; j++) {
                if(edges[i].first == edges[j].first || edges[i].first == edges[j].second || edges[i].second == edges[j].first || edges[i].second == edges[j].second) {
                    adj[i + 1][j + 1] = 1;
                    adj[j + 1][i + 1] = 1;
                }
            }
        }
    }

    MPI_Bcast(&M, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    MPI_Bcast(adj, 501 * 501, MPI_INT, root_rank, MPI_COMM_WORLD);
    for(int i = 0; i <= M; i++) {
        color[i] = -1;
        for(int j = 0; j <= M; j++) {
            available_colors[i][j] = 1;
        }
    }
    int per_process = M / numprocs;
    int left_over = M % numprocs;

    int start_node = rank*per_process + min(left_over, rank) + 1;
    int end_node = start_node + per_process + (rank < left_over ? 1 : 0) - 1;
    int nodes_colored = 0;
    while(nodes_colored != M) {
        vector<pair<int, int>> color_these_nodes;
        for(int i = start_node; i <= end_node && i <= M; i++) {
            if(color[i] != -1) {
                continue;
            }
            bool can_color = true;
            for(int j = M; j > i; j--) {
                if(adj[i][j] && color[j] == -1) {
                    can_color = false;
                    break;
                }
            }
            if(can_color) {
                for(int j = 1; j <= M; j++) {
                    if(available_colors[i][j]) {
                        color[i] = j;
                        color_these_nodes.push_back({i, j});
                        break;
                    }
                }
            }
        }
        
        int to_send = color_these_nodes.size();
        int send_data[to_send][2];
        for(int i = 0; i < numprocs; i++) {
            int received_size;
            if(i == rank) {
                received_size = to_send;
            }
            MPI_Bcast(&received_size, 1, MPI_INT, i, MPI_COMM_WORLD);
            int received_data[received_size][2];
            if(i == rank) {
                for(int i = 0; i < received_size; i++) {
                    received_data[i][0] = color_these_nodes[i].first;
                    received_data[i][1] = color_these_nodes[i].second;
                }
            }
            MPI_Bcast(received_data, 2 * received_size, MPI_INT, i, MPI_COMM_WORLD);
            nodes_colored += received_size;
            for(int i = 0; i < received_size; i++) {
                int node = received_data[i][0];
                int remove_color = received_data[i][1];
                color[node] = remove_color;
                for(int j = 0; j <= 500; j++) {
                    if(adj[node][j]) {
                        available_colors[j][remove_color] = 0;
                    }
                }
            }
        }
    }
    
    if(rank == root_rank) {
        fstream out_file;
        out_file.open(argv[2], ios::out);
        set<int> s;
        for(int i = 1; i <= M; i++) {
            s.insert(color[i]);
        }
        out_file << s.size() << "\n";
        for(int i = 1; i <= M; i++) {
            out_file << color[i] << " ";
        } out_file << "\n";
        out_file.close();
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