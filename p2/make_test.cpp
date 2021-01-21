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
    srand (time(NULL));
    int N = 100000;
    vector<ll> v(N);
    for(int i = 0; i < N; i++) {
    	v[i] = rand() % 1000000000;
    }
    fstream in_file, ans;
    in_file.open("in.txt", ios::out);
    ans.open("ans.txt", ios::out);
    in_file << N << "\n";
    for(int i = 0; i < N; i++) {
    	in_file << v[i] << " ";
    }
    sort(all(v));
    for(int i = 0; i < N; i++) {
    	ans << v[i] << " ";
    }
    ans.close();
    in_file.close();
}