/*
 *  SRC: POJ 3264
 * PROB: Balanced Lineup
 * ALGO: Segment Tree
 * DATE: Jul 21, 2011
 * COMP: g++
 *
 * Created by Leewings Ac
 */

#include <cstdio>
#include <algorithm>

using namespace std;

class Segment_Tree {
    private:
        const static int INF = 0x3f3f3f3f;

        int LC(int x) { return x << 1; }
        int RC(int x) { return (x << 1) | 1; }

        struct Tnode {
            int a, b;
            int mn, mx; // min, max
        };
        Tnode node[200000];

    public:
        void build(int a, int b, int idx = 1)
        {
            node[idx].a = a;
            node[idx].b = b;
            node[idx].mn = INF;
            node[idx].mx = -INF;

            if (a + 1 < b) {
                build(a, (a + b) >> 1, LC(idx));
                build((a + b) >> 1, b, RC(idx));
            }
        }

        void insert(int c, int d, int v, int idx = 1)
        {
            if (c <= node[idx].a && node[idx].b <= d) {
                node[idx].mn = min(node[idx].mn, v);
                node[idx].mx = max(node[idx].mx, v);
                return ;
            }

            if (c < (node[idx].a + node[idx].b) >> 1) insert(c, d, v, LC(idx));
            if (d > (node[idx].a + node[idx].b) >> 1) insert(c, d, v, RC(idx));
            node[idx].mn = min(node[LC(idx)].mn, node[RC(idx)].mn);
            node[idx].mx = max(node[LC(idx)].mx, node[RC(idx)].mx);
        }

        void query(int c, int d, int* mn, int* mx, int idx = 1)
        {
            if (c <= node[idx].a && node[idx].b <= d) {
                *mn = node[idx].mn;
                *mx = node[idx].mx;
                return ;
            }

            int lmn = INF, lmx = -INF, rmn = INF, rmx = -INF;
            if (c < (node[idx].a + node[idx].b) >> 1) query(c, d, &lmn, &lmx, LC(idx));
            if (d > (node[idx].a + node[idx].b) >> 1) query(c, d, &rmn, &rmx, RC(idx));
            *mn = min(lmn, rmn);
            *mx = max(lmx, rmx);
        }
};

Segment_Tree st;

int main()
{
    int n, q;
    scanf("%d%d", &n, &q);

    st.build(0, n);

    for (int i = 0; i < n; i++) {
        int h;
        scanf("%d", &h);
        st.insert(i, i + 1, h);
    }

    for (int i = 0; i< q; i++) {
        int a, b;
        scanf("%d%d", &a, &b);

        int mn, mx;
        st.query(a - 1, b, &mn, &mx);

        printf("%d\n", mx - mn);
    }

    return 0;
}
