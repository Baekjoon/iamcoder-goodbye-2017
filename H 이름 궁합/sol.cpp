#include <cstdio>
#include <cstring>

const int MN = 2005, alp[] = {3, 2, 1, 2, 3, 3, 2, 3, 3, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 1, 1, 2, 2, 1};
int n, s[2 * MN][2];
char a[MN], b[MN];

int main(){
	scanf("%s%s", a, b);
	n = strlen(a);
	for(int i = 0; i < n; i++){
		s[2 * i][0] = alp[a[i] - 'A'];
		s[2 * i + 1][0] = alp[b[i] - 'A'];
	}
	for(n *= 2; n > 2; n--){
		int p = n & 1;
		for(int i = 0; i < n - 1; i++) s[i][!p] = (s[i][p] + s[i + 1][p]) % 10;
	}
	printf("%d%d\n", s[0][0], s[1][0]);
}