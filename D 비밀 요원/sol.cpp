#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pp;
typedef pair<ll,ll> pll;
void read(int& x){ assert(scanf("%d",&x) == 1); }
void read(ll& x){ assert(scanf("%lld",&x) == 1); }
template<typename T,typename... Args>
void read(T& a,Args&... b){ read(a); read(b...); }
#define all(x) (x).begin(),(x).end()
#define pb push_back
#define eb emplace_back
#define x first
#define y second

const int MAX_V = 200000 + 10;
const int MAX_Q = 200000 + 10;
const int MAX_E = MAX_V + 100 + 10;

int n, m;
int Q;

pp d[MAX_V];
pp e[MAX_E];
int ec[MAX_E];
pp q[MAX_Q];

void in(){
    read(n, m);
    for(int i=1; i<=n; ++i){
        int x, y; read(x, y); d[i]=pp{x, y};
    }
    for(int i=1; i<=m; ++i){
        int a, b, c; read(a, b, c); e[i]=pp{a, b}; ec[i] = c;
    }
    read(Q);
    for(int i=1; i<=Q; ++i){
        int x, y; read(x, y); q[i]=pp{x, y};
    }
}

vector<int> line_new[MAX_V];
vector<int> line_bye[MAX_V];

map<pp,int> edge_index;
double current_x;
struct line {
    double x, y;
    int e;
    bool operator<(const line& R) const {
        if(x == R.x) return y<R.y;
        return x < R.x;
    }
};
struct line_sorter {
    bool operator()(const line& a, const line& b){
        return (a.x-b.x)*current_x < b.y-a.y;
    }
};
set<line, line_sorter> sline;
line get_line(int i, int j){
    if(i>j) swap(i, j); // for consistent result
    double grad = d[j].y-d[i].y;
    assert(d[j].x != d[i].x);
    grad /= d[j].x-d[i].x;
    double yint = d[i].y-grad*d[i].x;
    return {grad, yint, edge_index[pp{i, j}]};
}

map<int, vector<pp>> occasion;

int par[MAX_E * 2];
int sz [MAX_E * 2];
int root(int x){ return (x==par[x])?x:(par[x]=root(par[x])); }
void Join(int a, int b){
    a = root(a); b = root(b);
    if(a == b) return;
    par[a] = b; sz[b] += sz[a];
}
int soto;

pp operator-(pp a, pp b){
    return {a.x-b.x, a.y-b.y};
}

bool ccw(pp A, pp B){
    return B.y*1LL*A.x - B.x*1LL*A.y > 0;
}
bool ccw(pp A, pp B, pp C){
    return ccw(B-A, C-A);
}
double Atan(pp t){ return atan2(t.y, t.x); }

vector<int> adj[MAX_V];
void block(){
    for(int i=1; i<=2*m; ++i) par[i] = i, sz[i]=1;
    soto = 2*m+1; par[soto] = soto; sz[soto]=1;
    for(int i=1; i<=m; ++i){
        int a, b; tie(a, b) = e[i];
        edge_index[pp{a, b}] = i;
        edge_index[pp{b, a}] = i;
        adj[a].pb(b); adj[b].pb(a);
    }
    for(int i=1; i<=n; ++i){
        auto& neigh = adj[i];
        sort(all(neigh), [&](const int& a, const int& b){
            double theta_a = Atan(d[a]-d[i]);
            double theta_b = Atan(d[b]-d[i]);
            if(abs(theta_a - theta_b) < 1e-8){
                return ccw(d[i], d[a], d[b]);
            } else return theta_a < theta_b;
        });
        int J = neigh.size();
        for(int j=0; j<J; ++j){
            int p = neigh[j];
            int pi = edge_index[pp{i, p}];
            int q = neigh[(j+1)%J];
            int qi = edge_index[pp{i, q}];
            p = (d[p]>d[i] ? 2*pi-1 : 2*pi);
            q = (d[q]>d[i] ? 2*qi : 2*qi-1);
            Join(p, q);
        }
    }
}

int qwhere[MAX_Q];

int shrink[MAX_E * 2];
int fn;
ll dst[110][110];

int main(int argc, char** argv){
    in();
    block();

    for(int i=1; i<=m; ++i){
        int a, b; tie(a, b) = e[i];
        if(d[a].x == d[b].x) continue;
        if(d[a] > d[b]) swap(a, b);
        line_new[a].pb(b);
        line_bye[b].pb(a);
    }

    for(int i=1; i<=n; ++i) occasion[d[i].x].eb(0, i);
    for(int i=1; i<=Q; ++i) occasion[q[i].x].eb(1, i);

    for(auto& TMP:occasion){
        int X = TMP.x;
        auto& v=TMP.y;

        current_x = X - 0.5;
        for(auto& tmp:v){
            if(tmp.first != 0) continue;
            int p = tmp.second;
            for(int q : line_bye[p]){
                sline.erase(get_line(p, q));
            }
        }

        current_x = X;
        for(auto& tmp:v){
            if(tmp.first != 1) continue;
            int p = tmp.second;
            line ql{0., (double)q[p].y, 0};
            auto it = sline.lower_bound(ql);
            if(it == sline.end()){
                qwhere[p] = soto;
            } else {
                qwhere[p] = 2*it->e;
            }
        }

        current_x = X + 0.5;
        for(auto& tmp:v){
            if(tmp.first != 0) continue;
            int p = tmp.second;
            for(int q : line_new[p]){
                sline.insert(get_line(p, q));
            }
        }
    }

// Find outer hull and connect it to soto.
    int minp = min_element(d+1, d+n+1) - d;
    int minp_up = *max_element(all(adj[minp]),
        [&](const int& a, const int& b){
            double theta_a = Atan(d[a]-d[minp]);
            double theta_b = Atan(d[b]-d[minp]);
            if(abs(theta_a - theta_b) < 1e-8){
                return ccw(d[minp], d[a], d[b]);
            } else return theta_a < theta_b;
        });
    Join(soto, edge_index[pp{minp, minp_up}]*2-1);

    for(int i=1; i<=2*m+1; ++i){
        int j = root(i);
        if(shrink[j]) continue;
        shrink[j] = ++fn;
    }

    const ll inf = 1ll<<60;
    for(int i=1; i<=fn; ++i) for(int j=1; j<=fn; ++j){
        if(i!=j) dst[i][j] = inf;
    }
    
    for(int i=1; i<=m; ++i){
        int a = shrink[root(2*i-1)];
        int b = shrink[root(2*i)];
        if(a != b){
            dst[a][b] = dst[b][a] = min(dst[a][b], (ll) ec[i]);
        }
    }

    for(int j=1; j<=fn; ++j){
        for(int i=1; i<=fn; ++i){
            for(int k=1; k<=fn; ++k){
                dst[i][k] = min(dst[i][k], dst[i][j] + dst[j][k]);
            }
        }
    }

    int cp = shrink[root(soto)];
    for(int i=1; i<=Q; ++i){
        int np = shrink[root(qwhere[i])];
        printf("%lld\n", dst[cp][np]);
        cp = np;
    }
    return 0;
}
