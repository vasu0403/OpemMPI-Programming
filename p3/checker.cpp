#include "bits/stdc++.h"
#include "ext/pb_ds/assoc_container.hpp"
#include "ext/pb_ds/tree_policy.hpp"

using namespace std;
using namespace __gnu_pbds;


#define ll long long int
#define pb push_back
#define mp make_pair
#define ff first
#define ss second
#define all(a) a.begin(),a.end()

typedef tree <ll, null_type, less <ll>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
// order_of_key(val): returns the number of values less than val
// find_by_order(k): returns an iterator to the kth largest element (0-based)
vector<int> adj[101];
int color[101];
int main(void)
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    fstream graph;
    graph.open("in.txt", ios::in);
    int N, M;
    graph >> N >> M;
    int deg = 0;
    for(int i = 0; i < M; i++) {
    	int a, b;
    	graph >> a >> b;
    	adj[a].pb(b);
    	adj[b].pb(a);
    }
    for(int i = 1; i <= N; i++) {
    	deg = max(deg, (int)adj[i].size());
    }
    int max_allowed = deg + 1;
    fstream ans;
    ans.open("out.txt", ios::in);
    int used;
    ans >> used;
    if(used > max_allowed) {
    	cout << "WRONG: more than " << used <<" colors used\n";
    	return 0;
    }
    for(int i = 1; i <= N; i++) {
    	ans >> color[i];
    	if(color[i] > max_allowed) {
    		cout << "WRONG\n";
    		return 0;
    	}
    }
    for(int i = 1; i <= N; i++) {
    	for(int j: adj[i]) {
    		if(color[i] == color[j]) {
    			cout << "WRONG: color of " << i << " and " << j << " is same but they share an edge\n";
    			return 0;
    		}
    	}
    }
    cout << "CORRECT\n";
    return 0;	
}