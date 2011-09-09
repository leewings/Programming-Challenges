/*
 *  SRC: POJ 2135
 * PROB: Farm Tour
 * ALGO: MCMF
 * DATE: Jul 25, 2011 
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
    int cpf; // cost per flow

    Edge(int _v, int _p, int _c, int _cpf)
        : v(_v), p(_p), c(_c), f(0), cpf(_cpf)
    { }
};

const int INF = 0x3fffffff;
const int MAXN = 1010;
const int orig = 0, dest = MAXN;

vector<Edge> edge[MAXN + 1];

struct Route {
    int u, v, p;

    Route(int uu = 0, int vv = 0, int pp = 0)
        : u(uu), v(vv), p(pp)
    { }
} bfs_route[MAXN + 1];

bool vis[MAXN + 1];
int dist[MAXN + 1];

inline int fmin(int a, int b)
{
    return a < b ? a : b;
}

inline void add_edge(int u, int v, int capa, int cpf)
{
    edge[u].push_back(Edge(v, edge[v].size(), capa, cpf));
    edge[v].push_back(Edge(u, edge[u].size() - 1, 0, -cpf));
}

void build_graph()
{
    int n, m;
    scanf("%d%d", &n, &m);
    add_edge(orig, 1, 2, 0);
    for (int i = 0; i < m; i++) {
        int s, e, v;
        scanf("%d%d%d", &s, &e, &v);
        add_edge(s, e, 1, v);
        add_edge(e, s, 1, v);
    }
    add_edge(n, dest, 2, 0);
}

bool spfa()
{
    memset(vis, false, sizeof(vis));
    for (int i = 0; i <= MAXN; i++) dist[i] = INF;

    queue<int> Q;
    Q.push(orig);
    vis[orig] = true;
    dist[orig] = 0;
    bfs_route[orig] = Route(-1, 0, -1);

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();
        vis[u] = false;
        for (unsigned int i = 0; i < edge[u].size(); i++) {
            int v = edge[u][i].v;
            int dis = edge[u][i].cpf;
            if (dis + dist[u] < dist[v] && edge[u][i].f < edge[u][i].c) {
                dist[v] = dis + dist[u];
                bfs_route[v] = Route(u, v, i);
                if (vis[v] == false) {
                    vis[v] = true;
                    Q.push(v);
                }
            }
        }
    }

    return dist[dest] < INF;
}

int flow()
{
    int min_flow = INF;
    Route r = bfs_route[dest];
    while (r.u != -1) {
        min_flow = fmin(min_flow, edge[r.u][r.p].c - edge[r.u][r.p].f);
        r = bfs_route[r.u];
    }

    int res = 0;
    r = bfs_route[dest];
    while (r.u != -1) {
        edge[r.u][r.p].f += min_flow;
        res += edge[r.u][r.p].cpf;
        int j = edge[r.u][r.p].p;
        edge[r.v][j].f = -edge[r.u][r.p].f;
        r = bfs_route[r.u];
    }
    res *= min_flow;

    return res;
}

int mcmf()
{
    int res = 0;
    while (spfa()) res += flow();

    return res;
}

int main()
{
    build_graph();

    printf("%d\n", mcmf());

    return 0;
}