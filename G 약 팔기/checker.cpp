#include <cstdio>
#include <algorithm>
#include <cassert>
using namespace std;

const int MAX_N = 1000000, MAX_K = 2000;
int a[MAX_K];
bool chk[MAX_N + 1];

int main(int argc, char **argv){
	
	FILE *in = fopen(argv[1], "r");
	FILE *sol = fopen(argv[2], "r");
	FILE *out = fopen(argv[3], "r");
	
	int n;
	fscanf(in, "%d", &n);
	fclose(in);
	
	int max_k;
	fscanf(sol, "%d", &max_k);
	fclose(sol);
	
	int your_k;
	assert(fscanf(out, "%d" , &your_k) == 1);
	assert(1 <= your_k && your_k <= max_k);
	
	for(int i = 0; i < your_k; i++){
		assert(fscanf(out, "%d", a + i) == 1);
		assert(1 <= a[i] && a[i] <= MAX_N);
		int cur_sum = 0;
		for(int j = i; j >= 0; j--){
			cur_sum += a[j];
			if(cur_sum <= n) chk[cur_sum] = true;
		}
	}

	assert(count(chk + 1, chk + n + 1, true) == n);
	assert(fscanf(out, "%d", &n) == -1);
	
	return 0;
}