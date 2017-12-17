#include <cstdio>
#include <vector>
using namespace std;
typedef long long ll;

const int MN = 200005, M = int(1e9) + 7;

ll pw(ll x, ll k){
	if(k == 0) return 1;
	return pw(x * x % M, k / 2) * (k % 2 ? x : 1LL) % M;
}
ll inv(ll x){ return pw(x, M - 2); }

int n;
ll a, b, sz[MN], q[MN], cc[MN], chk[MN], ans;
vector<int> e[MN];

int sf(int x, int p){
	sz[x] = 1;
	for(auto &i : e[x]) if(!chk[i] && i != p) sz[x] += sf(i, x);
	return sz[x];
}

int cf(int x, int p, int t){
	for(auto &i : e[x]) if(!chk[i] && i != p && sz[i] > t / 2) return cf(i, x, t);
	return x;
}

void g(int x, int p, int d){
	cc[d]++;
	for(auto &i : e[x]) if(!chk[i] && i != p) g(i, x, d + 1);
}

void f(int x){
	x = cf(x, 0, sf(x, 0));
	chk[x] = 1;
	ll s = 0, t = 1;
	for(auto &i : e[x]){
		if(chk[i]) continue;
		g(i, x, 1);
		ll ns = s, nt = t;
		for(int j = 1; cc[j]; j++){
			ans = (ans + (s + j * t) % M * q[j + 1] % M * cc[j]) % M;
			ns = (ns + q[j] * j % M * cc[j]) % M;
			nt = (nt + q[j] * cc[j]) % M;
			cc[j] = 0;
		}
		s = ns;
		t = nt;
	}
	for(auto &i : e[x]) if(!chk[i]) f(i);
}

int main(){
	scanf("%d%lld%lld", &n, &a, &b);
	q[0] = 1;
	q[1] = (b - a) * inv(b) % M;
	for(int i = 2; i <= n; i++) q[i] = q[i - 1] * q[1] % M;
	for(int i = 0, x, y; i < n - 1; i++){
		scanf("%d%d", &x, &y);
		e[x].push_back(y);
		e[y].push_back(x);
	}
	f(1);
	printf("%lld\n", ans);
}
