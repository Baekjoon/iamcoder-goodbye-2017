#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,ll> pp;
void read(int& x){ scanf("%d",&x); }
void read(ll& x){ scanf("%lld",&x); }
template<typename T,typename... Args>
void read(T& a,Args&... b){ read(a); read(b...); }
#define all(x) (x).begin(),(x).end()
#define pb push_back
#define eb emplace_back
#define x first
#define y second

/// Math libs.
pp operator+(pp a, pp b){ return {a.x+b.x, a.y+b.y}; }
pp operator-(pp a, pp b){ return {a.x-b.x, a.y-b.y}; }
pp operator-(pp a){ return {-a.x, -a.y}; }
ll ccw(pp a, pp b){ return a.x*b.y - a.y*b.x; }
ll ccw(pp a, pp b, pp c){ return ccw(b-a, c-a); }

pp turn_cw(pp p){
	return {p.y, -p.x};
}
pair<int,pp> analyze(const pp& a){
	if(a.x == 0 && a.y == 0) return {0, {0, 0}};
	pp t = a;
	int c = 1;
	for(;;){
		if(t.x>0 && t.y>=0) break;
		++c; t=turn_cw(t);
	}
	return {c, t};
}
bool cmp_theta(const pp& a, const pp& b){
	auto aa=analyze(a);
	auto ab=analyze(b);
	if(aa.x != ab.x) return aa.x < ab.x;
	return ccw(aa.y, ab.y) > 0;
}
auto cmp_diff_exact = [&](const pair<pp,pp>& a, const pair<pp,pp>& b){
	bool right = cmp_theta(a.x, b.x);
	bool wrong = cmp_theta(b.x, a.x);
	if(right || wrong) return right || !wrong;
	return a.y < b.y;
};
auto cmp_diff_angle = [&](const pair<pp,pp>& a, const pair<pp,pp>& b){
	return cmp_theta(a.x, b.x);
};

/// Data
int n;
pp p[500];
pair<pp,pp> diff[301*301+10];
int dn;

/// current starting point
int s;

const int Mod = int(1e9) + 7;

struct smart_tree {
	int m;
	int di[500], din;
    int dpval[500];
    int pl, pr, cs;

	void clear(){
        pl=pr=cs=0;
        memset(dpval, 0, sizeof(dpval));
	}

	void init(int I){
		m = I; din = 0;
		for(int i=1; i<=n; ++i) if(i != m){
			di[din++] = lower_bound(diff, diff+dn, pair<pp,pp>{p[m]-p[i], {i, m}}, cmp_diff_exact)-diff;
		}
		sort(di, di+din);
	}

	int range_sum(int l, int r){
		l = lower_bound(di, di+din, l) - di;
		r = upper_bound(di, di+din, r) - di - 1;
		if(l>r) return 0;
        while(pl<l){
            cs += Mod-dpval[pl++]; cs %= Mod;
        }
        while(r<pr){
            cs += Mod-dpval[pr--]; cs %= Mod;
        }
        while(l<pl){
            cs += dpval[--pl]; cs %= Mod;
        }
        while(pr<r){
            cs += dpval[++pr]; cs %= Mod;
        }
        return cs;
	}
	
	void update(int val, int dii){
		int p = lower_bound(di, di+din, dii) - di;
        if(pl <= p && p <= pr){
        	cs += Mod - dpval[p]; cs %= Mod;
        }
        dpval[p]=val;
        if(pl <= p && p <= pr){
        	cs += val; cs %= Mod;
        }
	}
} tree[301];

int main()
{
	read(n);
	for(int i=1; i<=n; ++i){ int x, y; read(x, y); p[i]={x, y}; }
	for(int i=1; i<=n; ++i){
		for(int j=1; j<=n; ++j){
			if(i==j) continue;
			diff[dn++] = {p[j]-p[i], {i, j}};
		}
	}
	sort(diff, diff+dn, cmp_diff_exact);
	
	/*for(int i=0; i<dn; ++i){
		printf("Line nb. %2d :  connects  %3d and %3d\n", i, diff[i].y.x, diff[i].y.y);
	}*/
	
	for(int i=1; i<=n; ++i) tree[i].init(i);
	
	ll ans = 0;
	for(s=1; s<=n; ++s){
		//printf("--------------------\n");
		//printf("Starting point %d\n", s);
		for(int i=1; i<=n; ++i) tree[i].clear();
		int lbnd = 0;
		for(int li=0; li<dn; ++li){
			pp vec = diff[li].x;
			int from, to; tie(from, to) = diff[li].y;
			
			if(cmp_theta(-vec, vec)) while(lbnd < li && ccw(vec, diff[lbnd].x) >= 0) ++lbnd;
			
			ll dp = tree[from].range_sum(lbnd, li-1);
			if(from == s) dp=(dp+1)%Mod;
			
			if(dp || 1){
				//printf("Line %d -> %d : %-10lld (bring %-5d from %d to %d)\n", from, to, dp, tree[from].range_sum(lbnd, li-1), lbnd, li-1);
			}
			
			tree[to].update(dp, li);
		}
		//printf("Total : %-10d\n", tree[s].range_sum(0, dn-1));
		ans += tree[s].range_sum(0, dn-1);
		ans %= Mod;
	}
	
	printf("%lld\n", ans);
	return 0;
}
