/*
 *  SRC: POJ 3281
 * PROB: Dining
 * ALGO: Dinic
 * DATE: Jun 2, 2011 
 * COMP: g++
 *
 * Created by Leewings Ac
 */

#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>

using std::vector;
using std::queue;

struct Edge {
    int v, p;
    int c, f; // capa, flow

    Edge(int _v, int _p, int _c)
        : v(_v), p(_p), c(_c), f(0)
    { }
};

const int INF = 0x3fffffff;
const int MAX_N = 1000;
const int src = 0, dest = MAX_N;

vector<Edge> edge[MAX_N + 1];
int lev[MAX_N + 1];

int N, F, D;

inline int fmin(int a, int b)
{
    return a < b ? a : b;
}

inline void addEdge(int u, int v, int capa)
{
    edge[u].push_back(Edge(v, edge[v].size(), capa));
    edge[v].push_back(Edge(u, edge[u].size() - 1, 0));
}

void buildGraph()
{
    scanf("%d%d%d", &N, &F, &D);

    /*
     * 0: src
     * 1 to F: FF
     * F + 1 to F + N: C1
     * F + N + 1 to F + 2N: C2
     * F + 2N + 1 to F + 2N + D: DD
     * MAX_N: dest
     */

    const int FF = 0;
    const int C1 = F, C2 = F + N;
    const int DD = F + 2 * N;

    for (int i = 1; i <= F; i++) {
        // src -> FF, capa = 1
        // FF -> src, capa = 0
        addEdge(src, FF + i, 1);
    }
    for (int i = 1; i <= N; i++) {
        // C1 -> C2, capa = 1
        // C2 -> C1, capa = 0
        addEdge(C1 + i, C2 + i, 1);
    }
    for (int i = 1; i <=D; i++) {
        // DD -> dest, capa = 1
        // dest -> DD, capa = 0
        addEdge(DD + i, dest, 1);
    }

    for (int i = 1; i <= N; i++) {
        int f, d;
        scanf("%d%d", &f, &d);
        for (int j = 0; j < f; j++) {
            int tmp;
            scanf("%d", &tmp);
            // FF -> C1, capa = 1
            // C1 -> FF, capa = 0
            addEdge(FF + tmp, C1 + i, 1);
        }
        for (int j = 0; j < d; j++) {
            int tmp;
            scanf("%d", &tmp);
            // C2 -> DD, capa = 1
            // DD -> C2, capa = 0
            addEdge(C2 + i, DD + tmp, 1);
        }
    }
}

bool bfs()
{
    memset(lev, -1, sizeof(lev));

    queue<int> Q;
    Q.push(src);
    lev[src] = 0;

    while(!Q.empty()) {
        int u = Q.front();
        Q.pop();
        for (int i = 0; i < edge[u].size(); i++) {
            int v = edge[u][i].v;
            if (lev[v] == -1 && edge[u][i].f < edge[u][i].c) {
                lev[v] = lev[u] + 1;
                Q.push(v);
            }
        }
    }

    return lev[dest] > -1;
}

int dfs(int u, int f)
{
    if (u == dest) return f;

    int res = 0;
    for (int i = 0; i < edge[u].size(); i++) {
        int v = edge[u][i].v;
        if (lev[v] == lev[u] + 1 && edge[u][i].f < edge[u][i].c) {
            int tmp = dfs(v, fmin(f - res, edge[u][i].c - edge[u][i].f));
            res += tmp;
            edge[u][i].f += tmp;
            int j = edge[u][i].p;
            edge[v][j].f = -edge[u][i].f;
            if (res == f) break;
        }
    }

    return res;
}

int dinic()
{
    int res = 0;
    while (bfs()) {
        int tmp = dfs(src, MAX_N);
        if (tmp) res += tmp;
        else break;
    }

    return res;
}

int main()
{
    buildGraph();

    printf("%d\n", dinic());

    return 0;
}