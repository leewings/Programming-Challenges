/*
 *  SRC: ZOJ 3545
 * PROB: Rescue the Rabbit
 * ALGO: DP on AC Automaton
 * DATE: Oct 05, 2011
 * COMP: g++
 *
 * Created by Leewings Ac
 */

#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>

using std::queue;
using std::max;

int n, l;
int w[20];
char dna[20][200];

inline int gene_to_id(char c)
{
    switch (c) {
        case 'A': return 0;
        case 'G': return 1;
        case 'T': return 2;
        case 'C': return 3;
    }
    return 0;
}

class ACAutomaton {
    private:
        const static int CHARSET_SIZE = 4;
        const static int NODE_MAX_SIZE = 1024;

        struct Tnode {
            Tnode *next[CHARSET_SIZE];
            Tnode *fail;
            int id;
        };
        Tnode *root;

        int node_cnt;
        Tnode node[NODE_MAX_SIZE];

        int f[2][1024][1024];
        bool vis[2][1024][1024];
        int curr, next;

    public:
        ACAutomaton() { reset(); }

        void reset()
        {
            memset(node, 0, sizeof(node));
            node_cnt = 0;
            root = &node[node_cnt++];

            memset(f, 0xc0, sizeof(f));
            memset(vis, 0, sizeof(vis));
            curr = 0, next = 1;
        }

        int insert(char *s, int id)
        {
            Tnode *p = root;

            while (*s) {
                int idx = gene_to_id(*s);
                if (!p->next[idx]) {
                    p->next[idx] = &node[node_cnt++];
                }
                p = p->next[idx];
                s++;
            }

            p->id = id;

            return true;
        }

        void build_fail()
        {
            queue<Tnode *> Q;

            for (int i = 0; i < CHARSET_SIZE; i++) {
                if (root->next[i]) {
                    root->next[i]->fail = root;
                    Q.push(root->next[i]);
                } else {
                    root->next[i] = root;
                }
            }

            while (!Q.empty()) {
                Tnode *curr = Q.front();
                Q.pop();

                for (int i = 0; i < CHARSET_SIZE; i++) {
                    if (curr->next[i]) {
                        curr->next[i]->fail = curr->fail->next[i];
                        Q.push(curr->next[i]);
                    } else {
                        curr->next[i] = curr->fail->next[i];
                    }
                }

                // for nesting case
                if (!curr->id) curr->id = curr->fail->id;
            }
        }

        void query()
        {
            int ans = 0xc0c0c0c0;

            f[curr][0][0] = 0;
            vis[curr][0][0] = 1;
            for (int i = 0; i < l; i++) {
                for (int j = 0; j < node_cnt; j++) {
                    for (int k = 0, final = 1 << n; k < final; k++) {
                        if (vis[curr][j][k]) {
                            for (int idx = 0; idx < CHARSET_SIZE; idx++) {
                                Tnode *p = &node[j];
                                p = p->next[idx];

                                int state = k,
                                    offset = 0;
                                Tnode *t = p;
                                while (t->id) {
                                    int t_state = 1 << (t->id - 1);
                                    if (!(state & t_state)) {
                                        offset += w[t->id];
                                        state |= t_state;
                                    }
                                    t = t->fail;
                                }

                                vis[next][p - node][state] = true;
                                f[next][p - node][state] = max(f[next][p - node][state], f[curr][j][k] + offset);
                                ans = max(ans, f[next][p - node][state]);
                            }
                        }
                    }
                }
                curr ^= 1;
                next ^= 1;
                memset(f[next], 0xc0, sizeof(f[next]));
                memset(vis[next], 0, sizeof(vis[next]));
            }

            if (ans < 0) puts("No Rabbit after 2012!");
            else printf("%d\n", ans);
        }
};

ACAutomaton ac;

int main()
{
    while (scanf("%d%d", &n, &l) != EOF) {
        for (int i = 1; i <= n; i++) {
            scanf("%s%d", dna[i], w + i);
            ac.insert(dna[i], i);
        }

        ac.build_fail();
        ac.query();
        ac.reset();
    }

    return 0;
}