#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll N = 200005, mod = 1e9+7;
ll n, sum[N], ans = 1;

int main()
{
	scanf("%lld",&n);
	for(ll i=1;i<=n;i++) {
		ll A;
		scanf("%lld",&A);
		sum[i+1] += 1;
		sum[min(i+A,n)+1] -= 1;
	}
	for(ll i=1;i<=n;i++) {
		sum[i] += sum[i-1];
		ans = ans * (sum[i] + 1) % mod;
	}
	printf("%lld\n",ans);
}
