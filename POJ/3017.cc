/*
 *  SRC: POJ 3017
 * PROB: Cut the Sequence
 * ALGO: DP
 * DATE: Aug 23, 2012
 * COMP: g++
 *
 * Created by Leewings Ac
 */

#include <cstdio>
#include <cstdlib>
#include <ctime>

const int MAXN = 100010;
const long long LLINF = 0x3f3f3f3f3f3f3f3fll;

inline long long MIN(long long x, long long y) { return x < y ? x : y; }

class Treap {
    private:
        struct Tnode {
            long long key;
            int fix;
            int cnt;

            Tnode *l, *r;
        };
        Tnode buf[MAXN];
        Tnode *root, *buftail;

        Tnode *left_rotate(Tnode *x)
        {
            Tnode *y = x->r;
            x->r = y->l;
            y->l = x;
            return y;
        }

        Tnode *right_rotate(Tnode *x)
        {
            Tnode *y = x->l;
            x->l = y->r;
            y->r = x;
            return y;
        }

        Tnode *insert(Tnode *x, long long key)
        {
            if (!x) {
                x = buftail++;
                x->key = key;
                x->fix = rand() * rand();
                x->cnt = 1;
                x->l = x->r = 0;
                return x;
            }

            if (key < x->key) {
                x->l = insert(x->l, key);
                if (x->l->fix > x->fix) x = right_rotate(x);
            } else if (key > x->key) {
                x->r = insert(x->r, key);
                if (x->r->fix > x->fix) x = left_rotate(x);
            } else {
                x->cnt++;
            }
            return x;
        }

        Tnode *erase(Tnode *x, long long key)
        {
            if (!x) return 0;
            if (key < x->key) x->l = erase(x->l, key);
            else if (key > x->key) x->r = erase(x->r, key);
            else {
                if (x->cnt > 1) {
                    x->cnt--;
                    return x;
                }

                if (!x->l && !x->r) return 0;
                if (!x->l) x = left_rotate(x);
                else if (!x->r) x = right_rotate(x);
                else if (x->l->fix < x->r->fix) x = left_rotate(x);
                else x = right_rotate(x);
                x = erase(x, key);
            }

            return x;
        }

    public:
        Treap() : root(0), buftail(buf) { }
        void insert(long long key) { root = insert(root, key); }
        void erase(long long key) { root = erase(root, key); }
        long long min_element()
        {
            if (!root) return LLINF;

            Tnode *x = root;
            while (x->l) x = x->l;
            return x->key;
        }
};

Treap priority;
long long a[MAXN], sum[MAXN], que[MAXN];
long long f[MAXN];

int main()
{
    // srand(time(0));

    int n;
    long long m;
    scanf("%d%lld", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%lld", a + i);
        if (a[i] > m) {
            puts("-1");
            return 0;
        }
    }
    for (int i = 1; i <= n; i++) {
        sum[i] = sum[i - 1] + a[i];
    }

    int head = 0, tail = 0;
    int lower = 1;
    for (int i = 1; i <= n; i++) {
        while (sum[i] - sum[lower - 1] > m) lower++;

        while (head < tail && que[head] < lower) {
            head++;
            if (head < tail) priority.erase(f[que[head - 1]] + a[que[head]]);
        }
        while (head < tail && a[que[tail - 1]] <= a[i]) {
            tail--;
            if (head < tail) priority.erase(f[que[tail - 1]] + a[que[tail]]);
        }
        if (head < tail) {
            priority.insert(f[que[tail - 1]] + a[i]);
        }
        que[tail++] = i;

        f[i] = MIN(f[lower - 1] + a[que[head]], priority.min_element());
    }

    printf("%lld\n", f[n]);

    return 0;
}

