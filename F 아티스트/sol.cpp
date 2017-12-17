#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;

int n, k;
pll d[5010];

void in(){
	scanf("%d %d", &n, &k);
	for(int i=1; i<=n; ++i){
		int a, b; scanf("%d%d", &a, &b);
		d[i] = {a, b};
	}
}

ll ans = 1e18;

pll tmp[5010];
pll Try(int ca, int cb){
	for(int i=1; i<=n; ++i){
		tmp[i] = {ca*d[i].first + cb*d[i].second, i};
	}
	sort(tmp+1, tmp+n+1);
	ll sa = 0, sb = 0;
	for(int i=1; i<=k; ++i){
		int a, b; tie(a, b) = d[tmp[i].second];
		sa += a; sb += b;
	}
	ans = min(ans, sa * sb);
	return {sa, sb};
}

void recursive(pll L, pll D){
	ll dx = (D.first - L.first), dy = (L.second - D.second);
	ll chi = L.first * dy + L.second * dx;
	pll p = Try(dy, dx);
	ll new_chi = p.first * dy + p.second * dx;
	if(chi == new_chi) return;
	recursive(L, p);
	recursive(p, D);
}

int main(int argc, char** argv){
	if(argc > 1) freopen(argv[1], "r", stdin);
	in();

	pll left = Try(1, 0);
	pll down = Try(0, 1);

	recursive(left, down);

	printf("%lld\n", ans);
	return 0;
}