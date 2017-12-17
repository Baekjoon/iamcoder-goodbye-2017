#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pp;
typedef pair<ll,ll> pll;
void read(int& x){ scanf("%d",&x); }
void read(ll& x){ scanf("%lld",&x); }
template<typename T,typename... Args>
void read(T& a,Args&... b){ read(a); read(b...); }
#define all(x) (x).begin(),(x).end()
#define pb push_back
#define eb emplace_back
#define x first
#define y second

using t3=tuple<ll,ll,ll>;

int n;

t3 line[5010];

t3 cross(t3 a, t3 b){
	ll ax, ay, az; tie(ax, ay, az) = a;
	ll bx, by, bz; tie(bx, by, bz) = b;
	return t3{ay*bz - az*by, ax*bz - az*bx, ax*by - ay*bx};
}

ll gcd(ll a, ll b){ return b?gcd(b, a%b):a; }

int main()
{
	scanf("%d", &n);
	for(int i=1; i<=n; ++i){
		ll type, a, b;
		read(type, a, b);
		if(type == 1){
			line[i] = t3{b, -a, 0};
		} else {
			line[i] = t3{a, b, -1};
		}
	}
	int reg = 1;
	for(int i=1; i<=n; ++i){
		vector<t3> v;
		for(int j=1; j<i; ++j){
			ll x, y, z;
			tie(x, y, z) = cross(line[i], line[j]);
			if(!z) continue;
			if(z<0){ x=-x; y=-y; z=-z; }
			ll g = gcd(gcd(abs(x), abs(y)), abs(z));
			x/=g; y/=g; z/=g;
			v.eb(x, y, z);
		}
		sort(all(v));
		int unique_pt = unique(all(v)) - v.begin();
		reg += unique_pt + 1;
	}
	printf("%d\n", reg);
	return 0;
}
