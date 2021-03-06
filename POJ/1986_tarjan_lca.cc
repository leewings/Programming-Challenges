/*
 *  SRC: POJ 1986
 * PROB: Distance Queries
 * ALGO: Tarjan-LCA (Lowest Common Ancestor)
 * DATE: May 28, 2012
 * COMP: g++
 *
 * Created by Leewings Ac
 */

#include <cstdio>
#include <cstring>
#include <vector>

using std::vector;

const int MAXN = 40000;

class DisjointSet {
    public:
        int a[MAXN];

        DisjointSet() { reset(); }
        void reset() { memset(a, 0xff, sizeof(a)); }

        int find(int u)
        {
            if (a[u] < 0) return u;
            return a[u] = find(a[u]);
        }

        void join(int u, int v)
        {
            int x = find(u),
                y = find(v);
            if (x != y) {
                a[x] += a[y];
                a[y] = x;
            }
        }
};

struct Edge {
    int v, d;
    Edge(int _v, int _d) : v(_v), d(_d) { }
};

typedef vector<Edge>::const_iterator vci;
vector<Edge> edge[MAXN];
vector<Edge> query[MAXN];
DisjointSet ds;
int father[MAXN], dist[MAXN], ans[MAXN];
bool vis[MAXN];

void tarjan(int u, int length)
{
    vis[u] = true;
    dist[u] = length;

    for (vci e = query[u].begin(); e != query[u].end(); e++) {
        if (vis[e->v]) {
            ans[e->d] = dist[u] + dist[e->v] - dist[ds.find(e->v)] * 2;
        }
    }

    for (vci e = edge[u].begin(); e != edge[u].end(); e++) {
        if (e->v != father[u]) {
            father[e->v] = u;
            tarjan(e->v, length + e->d);
        }
    }

    ds.join(father[u], u);
}

int main()
{
    int n;
    scanf("%d%*d", &n);
    for (int i = 0; i < n; i++) edge[i].clear();
    for (int i = 0; i < n; i++) query[i].clear();
    for (int i = 1; i < n; i++) {
        int u, v, d;
        scanf("%d%d%d%*s", &u, &v, &d);
        edge[u - 1].push_back(Edge(v - 1, d));
        edge[v - 1].push_back(Edge(u - 1, d));
    }

    int k;
    scanf("%d", &k);
    for (int i = 0; i < k; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        query[u - 1].push_back(Edge(v - 1, i));
        query[v - 1].push_back(Edge(u - 1, i));
    }

    memset(vis, false, sizeof(vis));
    father[0] = 0;
    tarjan(0, 0);
    for (int i = 0; i < k; i++) printf("%d\n", ans[i]);

    return 0;
}
