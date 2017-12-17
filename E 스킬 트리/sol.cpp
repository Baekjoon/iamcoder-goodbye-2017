#include <cstdio>
#include <algorithm>
using namespace std;

using ll = long long;

const ll M = int(1e9) + 7;
int n, flp;
ll a, b;

ll pw(ll x, ll k){
	if(k == 0) return 1;
	return pw(x * x % M, k / 2) * (k % 2 ? x : 1LL) % M;
}

ll inv(ll x){ return pw(x, M - 2); }

ll g(ll x, ll m){
	if(x == 1) return m % M;
	return (pw(x, m) - 1 + M) * inv(x - 1) % M;
}

ll f1(ll m){ return m % M * (m % M + 1) % M * inv(2) % M; }

ll f2(ll m){ return (m % M * pw(a, m) - g(a, m) + M) % M * inv(a - 1) % M; }

ll f3(ll m){ return (a * g(a, m) - b * g(b, m) % M + M) % M * inv(a - b) % M; }

int main(){
	scanf("%lld%lld%d", &a, &b, &n);
	if(a < b){ swap(a, b); flp = 1; }
	auto f = (a == b ? (a == 1 ? f1 : f2) : f3);
	for(ll x, y, m; n--; ){
		scanf("%lld%lld%lld", &x, &y, &m);
		if(flp) y = x + 1 - y;
		printf("%lld\n", pw(a, x - y) * pw(b, y - 1) % M * f(m) % M);
	}
	return 0;
}