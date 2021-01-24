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

int main(void)
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    fstream in1, in2;
    in1.open("in.txt", ios::in);
    in2.open("out.txt", ios::in);

    int N;
    in1 >> N;
    vector<int> v(N);
    for(int i = 0; i < N; i++) {
    	in2 >> v[i];
    }
    in1.close();
    in2.close();
    vector<int> sorted = v;
    sort(all(sorted));
    for(int i = 0; i < N; i++) {
    	if(sorted[i] != v[i]) {
    		cout << "WRONG\n";
    		return 0;
    	}
    }
    cout << "CORRECT\n";
}