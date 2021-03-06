/*
 *  SRC: POJ 2352
 * PROB: Stars
 * ALGO: BIT
 * DATE: Jul 20, 2011
 * COMP: g++
 *
 * Created by Leewings Ac
 */

#include <cstdio>
#include <algorithm>

using std::sort;

class BIT {
    private:
        const static int bound = 320010;
        int c[bound + 1];
        int bit_mask;

        // x must *not* be *zero*
        int lowbit(int x) { return x & -x; }

        void get_bit_mask()
        {
            int cnt = 0;
            for ( ; bound >> cnt; cnt++)
                ;

            bit_mask = 1 << (cnt - 1);
        }

    public:
        void update(int x, int v)
        {
            for ( ; x <= bound; x += lowbit(x))
                c[x] += v;
        }

        int sum(int x)
        {
            int res = 0;
            for ( ; x > 0; x -= lowbit(x))
                res += c[x];
            return res;
        }

        // get_bit_mask() first
        int find(int tot)
        {
            int res = 0, cnt = 0, bm = bit_mask;
            while (bm != 0) {
                if (res + bm < bound && cnt + c[res + bm] < tot) { // find the left one
             // if (res + bm <=bound && cnt + c[res + bm] <=tot)   // find the right one
                    res += bm;
                    cnt += c[res];
                }
                bm >>= 1;
            }

            return res + 1; // left
         // return res;     // right
        }
};
BIT bit;

struct Point {
    int x, y;

    bool operator<(const Point& other) const
    {
        if (x == other.x) return y < other.y;
        return x < other.x;
    }
} star[15010];

int n;
int level[15010];

int main()
{
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        scanf("%d%d", &star[i].x, &star[i].y);

    sort(star, star + n);

    for (int i = 0; i < n; i++) {
        bit.update(star[i].y + 1, 1);
        level[bit.sum(star[i].y + 1) - 1]++;
    }

    for (int i = 0; i < n; i++)
        printf("%d\n", level[i]);

    return 0;
}
