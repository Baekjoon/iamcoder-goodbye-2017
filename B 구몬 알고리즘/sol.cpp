#include <cstdio>
using namespace std;
typedef long long ll;

const int MN = 200005, MK = 32, M = ll(1e9) + 7, I2 = ll(5e8) + 4;
int n, l, r;
ll dp[MK][MN], s[MK][MN], i2[MK], ans;

int main(){
	scanf("%d%d%d", &n, &l, &r);
	i2[0] = 1;
	for(int i = 1; i < MK; i++) i2[i] = i2[i - 1] * I2 % M;
	dp[0][0] = dp[0][1] = 1;
	for(int i = 2; i <= n; i++) dp[0][i] = (dp[0][i - 1] + (i - 1) * dp[0][i - 2]) % M;
	if(l == 1) ans = dp[0][n];
	for(int i = 1; (1 << i) <= r; i++){
		ll s = 0;
		for(int j = 3; j <= n; j++){
			s = (s + (j - 2) * dp[i - 1][j - 3]) % M * (j - 1) % M;
			dp[i][j] = (dp[i][j - 1] + (j - 1) * dp[i][j - 2] + s) % M;
		}
		if(l <= (1 << i)) ans = (ans + dp[i][n] * i2[i]) % M;
	}
	printf("%lld\n", ans);
}