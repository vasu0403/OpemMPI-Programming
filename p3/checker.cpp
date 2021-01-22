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
vector<int> adj2[501];
int color[501];
int main(void)
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    fstream graph;
    graph.open("in.txt", ios::in);
    int N, M;
    graph >> N >> M;
    int deg = 0;
    map<pair<int, int>, int> m;
    vector<pair<int, int>> edges;
    for(int i = 0; i < M; i++) {
    	int a, b;
    	graph >> a >> b;
        if(a > b) {
            swap(a, b);
        }
        edges.pb({a, b});
        m[{a, b}] = i + 1;
    	adj[a].pb(b);
    	adj[b].pb(a);
    }
    for(int i = 0; i < M; i++) {
        for(int j = i + 1; j < M; j++) {
            if(edges[i].ff == edges[j].ff || edges[i].ff == edges[j].ss || edges[i].ss == edges[j].ff || edges[i].ss == edges[j].ss) {
                adj2[i + 1].pb(j + 1);
                adj2[j + 1].pb(i + 1); 
            }
        }
    }
    for(int i = 1; i <= N; i++) {
    	deg = max(deg, (int)adj[i].size());
    }
    for(int i = 1; i <= M; i++) {
        deg = max(deg, (int)adj2[i].size());
    }
    int max_allowed = deg + 1;
    fstream ans;
    ans.open("out.txt", ios::in);
    int used;
    ans >> used;
    if(used > max_allowed) {
    	cout << "WRONG: more than " << max_allowed <<" colors used\n";
    	return 0;
    }
    for(int i = 1; i <= M; i++) {
    	ans >> color[i];
    	if(color[i] > max_allowed) {
    		cout << "WRONG\n";
    		return 0;
    	}
    }
    for(int i = 1; i <= N; i++) {
    	set<int> s;
        for(int j: adj[i]) {
            pair<int, int> cur_edge = {min(i, j), max(i, j)};
            int ind = m[cur_edge];
            s.insert(color[ind]);    		
    	}
        if((int)s.size() != (int)adj[i].size()) {
            for(int j: adj[i]) {
                pair<int, int> cur_edge = {min(i, j), max(i, j)};
                int ind = m[cur_edge];
                cout << ind << " " << color[ind] << "\n";           
            }   
            cout << "WRONG: edges of vertex " << i << " have same color.\n";
            return 0; 
        }
    }
    cout << "CORRECT\n";
    return 0;	
}