/*
 *  SRC: POJ 2983
 * PROB: Is the Information Reliable?
 * ALGO: SPFA
 * DATE: Apr 12, 2012
 * COMP: g++
 *
 * Created by Leewings Ac
 */

#include <cstdio>
#include <cstring>
#include <queue>

using std::queue;

const int MAXV = 1010;
const int MAXE = 300010;

int dist[MAXV];
int cnt[MAXV];
bool in_queue[MAXV];

struct Edge {
    int v, d;
    Edge *next;
};
Edge e_buf[MAXE];
Edge *e_head[MAXV];
Edge *e_tail = e_buf;

inline void add_edge(int u, int v, int d)
{
    *e_tail = (Edge){v, d, e_head[u]};
    e_head[u] = e_tail++;
}

bool spfa(int n, int src)
{
    memset(dist, 0x3f, sizeof(dist));
    memset(cnt, 0, sizeof(cnt));
    memset(in_queue, false, sizeof(in_queue));
    dist[src] = 0;

    queue<int> que;
    que.push(src);
    while (!que.empty()) {
        int u = que.front();
        que.pop();
        in_queue[u] = false;

        for (Edge *e = e_head[u]; e; e = e->next) {
            int v = e->v;
            int d = e->d;
            if (d + dist[u] < dist[v]) {
                dist[v] = d + dist[u];
                if (!in_queue[v]) {
                    if (cnt[v]++ >= n) return false;
                    que.push(v);
                    in_queue[v] = true;
                }
            }
        }
    }

    return true;
}

int main()
{
    int n, m;
    while (~scanf("%d%d", &n, &m)) {
        memset(e_head, 0, sizeof(e_head));
        e_tail = e_buf;

        // some points may be isolate, so add a super source 0
        for (int i = 1; i <= n; i++) {
            add_edge(0, i, 0);
        }
        for (int i = 0; i < m; i++) {
            char c;
            int u, v;
            scanf(" %c%d%d", &c, &u, &v);
            if (c == 'P') {
                int x;
                scanf("%d", &x);
                add_edge(u, v, -x);
                add_edge(v, u, x);
            } else {
                add_edge(u, v, -1);
            }
        }

        // n points plus a super souce, the 0
        puts(spfa(n + 1, 0) ? "Reliable" : "Unreliable");
    }

    return 0;
}

