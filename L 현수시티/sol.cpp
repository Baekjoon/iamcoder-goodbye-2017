#include<bits/stdc++.h>
#define X first
#define Y second
using namespace std;
typedef pair<int,int> pii;
const int N = 200005, L = 20;

int n, m, k, cc, bc, ec, tc;
int off[N], dfn[N], low[N];
int es[N], ee[N], ts[N], te[N];
int cat[N], inv[N], bel[N], par[L][N], dep[N];
bool vis[N];

vector<pii> adj[N];
vector<int> tree[N], bcc[N], edg[N];

map<pii,int> wh;

struct pseg {
	int v[4*N], l;
	void init () {
		for(l=1;l<=m+5;l<<=1);
	}
	void upd (int P, int V) {
		P += l;
		while(P) {v[P] += V; P >>= 1;}
	}
	int lb (int I) {
		I += l;
		while(I&(I+1)) {
			if(v[I]) break;
			I = (I-1)/2;
		}
		if((I&(I+1)) == 0) return 0;
		while(I < l) {
			if(v[2*I+1]) I = 2*I+1;
			else I = 2*I;
		}
		return I - l;
	}
	int rb (int I) {
		I += l;
		while(I&(I-1)) {
			if(v[I]) break;
			I = (I+1)/2;
		}
		if((I&(I-1)) == 0) return m+1;
		while(I < l) {
			if(v[2*I]) I = 2*I;
			else I = 2*I+1;
		}
		return I - l;
	}
	int get (int S, int E) {
		S += l; E += l;
		int R = 0;
		while(S<=E) {
			if(S%2==1) {R += v[S]; S++;}
			if(E%2==0) {R += v[E]; E--;}
			S >>= 1; E >>= 1;
		}
		return R;
	}
} blk, col;

struct rseg {
	int v[4*N], l;
	void init () {
		for(l=1;l<=m+5;l<<=1);
	}
	void upd (int S, int E, int V) {
		S += l; E += l;
		while(S<=E) {
			if(S%2==1) {v[S] += V; S++;}
			if(E%2==0) {v[E] += V; E--;}
			S >>= 1; E >>= 1;
		}
	}
	int get (int P) {
		int R = 0; P += l;
		while(P) {R += v[P]; P >>= 1;}
		return R;
	}
} ext;

void calc (int C, int P) {
	dfn[C] = low[C] = ++cc;
	for(auto &Z : adj[C]) {
		int T = Z.X;
		if(T == P) continue;
		if(dfn[T]) {
			low[C] = min(low[C], dfn[T]);
		}
		else {
			calc(T, C);
			low[C] = min(low[C], low[T]);
		}
	}
}

void split (int C, int V) {
	vis[C] = true;
	for(auto &Z : adj[C]) {
		int T, I; tie(T, I) = Z;
		if(vis[T]) continue;
		if(dfn[C] <= low[T]) {
			bcc[bc].push_back(C);
			bcc[bc].push_back(T);
			edg[bc].push_back(I);
			split(T, bc++);
		}
		else {
			if(~V) {
				bcc[V].push_back(T);
				edg[V].push_back(I);
			}
			split(T, V);
		}
	}
}

void dfs (int I) {
	ts[I] = ++tc;
	for(auto &T : tree[I]) {
		dep[T] = dep[I] + 1;
		dfs(T);
	}
	te[I] = tc;
}

void block (int I, int V) {
	if(bcc[I].size() == 2) {
		int T = bcc[I][1];
		if(blk.get(es[I], ee[I]) >= 1) {
			ext.upd(ts[T], te[T], V);
		}
	}
	else {
		int S = blk.rb(es[I]) + 1, E = blk.lb(ee[I]);
		if(S>E) return;
		int A = bcc[I][S-es[I]], B = bcc[I][E-es[I]];
		col.upd(ts[A], V); col.upd(ts[B], -V);
		ext.upd(ts[B], te[B], V);
	}
}

void update (int I, int V) {
	block(cat[I], -1);
	blk.upd(inv[I], V - off[I]);
	off[I] = V;
	block(cat[I], 1);
}

void update (int I) {update(I, (off[I] ^ 1));}

bool valid (int A, int P) {
	int C = 0;
	C += col.get(1, ts[A]) - col.get(1, ts[P]);
	C += ext.get(ts[A]) - ext.get(ts[P]);
	return (C == 0);
}

bool query (int A, int B) {
	if(dep[A] < dep[B]) swap(A, B);
	int AA = A, BB = B;
	for(int i=L;i--;) {
		if(dep[AA] - dep[BB] >= (1<<i)) {
			AA = par[i][AA];
		}
	}
	if(AA == BB) return valid(A, AA);
	for(int i=L;i--;) {
		if(par[i][AA] != par[i][BB]) {
			AA = par[i][AA];
			BB = par[i][BB];
		}
	}
	if(bel[AA] != bel[BB]) {
		AA = par[0][AA]; BB = par[0][BB];
	}
	int I = bel[AA];
	int X = wh[{I, AA}], Y = wh[{I, BB}];
	if(X > Y) swap(X, Y);
	int T = blk.get(X, Y-1);
	if(T > 0 && blk.get(es[I], ee[I]) - T > 0) return false;
	return (valid(A, AA) && valid(B, BB));
}

int main()
{
	scanf("%d%d",&n,&m);
	blk.init(); col.init(); ext.init();
	for(int i=1;i<=m;i++) {
		int A, B;
		scanf("%d%d",&A,&B);
		adj[A].push_back({B,i});
		adj[B].push_back({A,i});
	}
	calc(1, -1);
	split(1, -1);
	for(int i=0;i<bc;i++) {
		if(bcc[i].size() > 2) {
			for(auto &T : adj[bcc[i].back()]) {
				if(T.X == bcc[i][0]) {
					edg[i].push_back(T.Y); break;
				}
			}
		}
		es[i] = ec+1;
		for(int j=0;j<edg[i].size();j++) {
			++ec;
			wh[{i, bcc[i][j]}] = ec;
			inv[edg[i][j]] = ec;
			cat[edg[i][j]] = i;
		}
		ee[i] = ec;
		for(int j=1;j<bcc[i].size();j++) {
			tree[bcc[i][0]].push_back(bcc[i][j]);
			par[0][bcc[i][j]] = bcc[i][0];
			bel[bcc[i][j]] = i;
		}
	}
	dfs(1);
	for(int i=1;i<L;i++) {
		for(int j=1;j<=n;j++) {
			par[i][j] = par[i-1][par[i-1][j]];
		}
	}
	for(int i=1;i<=m;i++) {
		int T;
		scanf("%d",&T);
		update(i, T^1);
	}
	scanf("%d",&k);
	while(k--) {
		int typ, A, B;
		scanf("%d",&typ);
		if(typ == 1) {
			scanf("%d",&A);
			update(A);
		}
		if(typ == 2) {
			scanf("%d%d",&A,&B);
			puts(query(A, B) ? "YES" : "NO");
		}
	}
}
