#include<cstdio>
#include<algorithm>

using namespace std;

const int N = 200005;

int n, m, a[N], t[N], ql, qr, qk, maxx;

void disc(){
	sort(t+1, t+n+1);
	int len = unique(t+1, t+n+1) - (t+1);
	for(int i = 1; i <= n; i++){
		int tmp = lower_bound(t+1, t+len+1, a[i]) - t;
		a[i] = tmp;
		maxx = max(maxx, a[i]);
	}
}

struct segTree{
	int l, r, lson, rson, size;
}tr[4000005];
int cnt, root[N];
void pushup(int id){
	tr[id].size = tr[tr[id].lson].size + tr[tr[id].rson].size;
}
void build(int id, int l, int r){
	tr[id].l = l; tr[id].r = r;
	if(tr[id].l == tr[id].r){ tr[id].size = 0; return; }
	tr[id].lson = ++cnt;
	tr[id].rson = ++cnt;
	int mid = (tr[id].l + tr[id].r) >> 1;
	build(tr[id].lson, l, mid);
	build(tr[id].rson, mid+1, r);
	pushup(id);
}
void add(int cur, int pre, int pos){ // build current tree which bases on previous one
	tr[cur] = tr[pre];
	if(tr[cur].l == tr[cur].r){ tr[cur].size++; return; }
	int mid = (tr[cur].l + tr[cur].r) >> 1;
	if(pos <= mid){
		tr[cur].lson = ++cnt;
		add(tr[cur].lson, tr[pre].lson, pos);
	}
	else{
		tr[cur].rson = ++cnt;
		add(tr[cur].rson, tr[pre].rson, pos);
	}
	pushup(cur);
}
int queryKth(int p, int q, int k){ // find the kth pos in (tr[q]-tr[p])
	if(tr[q].l == tr[q].r)	return tr[q].l;
	int leftSize = tr[tr[q].lson].size - tr[tr[p].lson].size;
	if(k <= leftSize)	return queryKth(tr[p].lson, tr[q].lson, k);
	else	return queryKth(tr[p].rson, tr[q].rson, k - leftSize);
}

int main(){
	scanf("%d%d", &n, &m);
	for(int i = 1; i <= n; i++)
		scanf("%d", &a[i]), t[i] = a[i];
	disc();
	build(0, 1, maxx); // build an empty tree
	for(int i = 1; i <= n; i++){
		root[i] = ++cnt;
		add(root[i], root[i-1], a[i]);
	}
	while(m--){
		scanf("%d%d%d", &ql, &qr, &qk);
		printf("%d\n", t[queryKth(root[ql-1], root[qr], qk)]);
	}
	return 0;
}