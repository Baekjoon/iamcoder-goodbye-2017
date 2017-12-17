#include <cstdio>
#include <algorithm>
using namespace std;

using ll = long long;

const int MN = 2005, sz = 2048, inf = int(1e9) + 5;
struct Seg{
	ll dat[2 * sz];
	void ini(){ fill(dat + 1, dat + 2 * sz, 0); }
	void upd(int x, ll v){
		x += sz; dat[x] = v;
		for(x /= 2; x; x /= 2) dat[x] = max(dat[2 * x], dat[2 * x + 1]);
	}
	ll get(int s, int e){
		ll ret = 0;
		for(s += sz, e += sz; s <= e; s /= 2, e /= 2){
			if(s % 2 == 1) ret = max(ret, dat[s++]);
			if(e % 2 == 0) ret = max(ret, dat[e--]);
		}
		return ret;
	}
} S;

struct Isl{ int x, y, v, h; };

int n, xx[MN], yy[MN], hh[MN], ans = inf;
ll m;
Isl a[MN];

int can(int s, int e){
	S.ini();
	for(int i = 1; i <= n; i++){
		if(a[i].h < s || a[i].h > e) continue;
		S.upd(a[i].y, S.get(1, a[i].y) + a[i].v);
	}
	return S.dat[1] >= m;
}

int main(){
	scanf("%d%lld", &n, &m);
	for(int i = 1; i <= n; i++){
		scanf("%d%d%d%d", &a[i].x, &a[i].y, &a[i].v, &a[i].h);
		xx[i] = a[i].x;
		yy[i] = a[i].y;
		hh[i] = a[i].h;
	}
	sort(xx + 1, xx + n + 1);
	sort(yy + 1, yy + n + 1);
	for(int i = 1; i <= n; i++){
		a[i].x = int(lower_bound(xx + 1, xx + n + 1, a[i].x) - xx);
		a[i].y = int(lower_bound(yy + 1, yy + n + 1, a[i].y) - yy);
	}
	sort(a + 1, a + n + 1, [](Isl p, Isl q){
		return (p.x == q.x) ? (p.y < q.y) : (p.x < q.x);
	});
	sort(hh + 1, hh + n + 1);
	for(int i = 1, j = 1; i <= n && j <= n; ){
		if(can(hh[i], hh[j])){
			ans = min(ans, hh[j] - hh[i]);
			i++;
		}
		else j++;
	}
	printf("%d\n", ans == inf ? -1 : ans);
}