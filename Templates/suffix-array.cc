/*
 *  SRC: POJ 2774
 * PROB: Long Long Message
 * ALGO: Suffix Array
 * DATE: Jul 22, 2011
 * COMP: g++
 *
 * Created by Leewings Ac
 */

#include <cstdio>
#include <cstring>
#include <algorithm>

using std::swap;

class Suffix_Array {
    // use Doubling Algorithm

    private:
        const static int MAXLEN = 200012;
        const static int MAXCHAR = 128;

        int *nrank;
        int rbuf[2][MAXLEN]; // used for ranking

        int cnt[MAXLEN]; // used for counting sort

        void csort(int k, int range)
        {
            int t = 0;
            for (int i = len - k; i < len; i++) nrank[t++] = i;
            for (int i = 0; i < len; i++)
                if (suff[i] >= k) nrank[t++] = suff[i] - k;

            for (int i = 0; i < len; i++) cnt[rank[i]]++;
            for (int i = 1; i < range; i++) cnt[i] += cnt[i - 1];
            for (int i = len - 1, j; i >= 0; i--) {
                j = nrank[i];
                suff[--cnt[rank[j]]] = j;
            }

            memset(cnt, 0, sizeof(int) * range);
        }

    public:
        char str[MAXLEN];
        int len;

        int suff[MAXLEN]; // suff[i]: the i-th *sorted* suffix
        int *rank;         // rank[i]: the rank of the i-th *original* suffix
        // suff[i] = j <=> rank[j] = i

        // LCP(i, j): the Longest Common Prefix of the i-th and j-th *sorted* suffixes
        int hgt[MAXLEN];  //  hgt[i]: LCP(i - 1, i)

        void reset()
        {
            memset(rbuf, 0, sizeof rbuf);
            memset(cnt, 0, sizeof cnt);
            memset(suff, 0, sizeof suff);
            memset(hgt, 0, sizeof hgt);
        }

        void build()
        {
            len = strlen(str);
            rank = rbuf[0];
            nrank = rbuf[1];

            for (int i = 0; i < len; i++) {
                rank[i] = str[i];
                suff[i] = i;
            }
            csort(0, MAXCHAR);

            for (int k = 1, maxrank = MAXCHAR; maxrank != len; k <<= 1) {
                csort(k, maxrank + 1);

                maxrank = nrank[suff[0]] = 1;
                for (int i = 1; i < len; i++) {
                    if (rank[suff[i - 1]] == rank[suff[i]] && rank[suff[i - 1] + k] == rank[suff[i] + k]) {
                        nrank[suff[i]] = maxrank;
                    } else {
                        nrank[suff[i]] = ++maxrank;
                    }
                }

                swap(rank, nrank);
            }
            for (int i = 0; i < len; i++) rank[i]--;
        }

        void calc_hgt()
        {
            for (int i = 0, j, k = 0; i < len; hgt[rank[i++]] = k)
                for (k ? k-- : 0, j = rank[i] ? suff[rank[i] - 1] : len; str[i + k] == str[j + k]; k++)
                    ;
        }
};

Suffix_Array sa;

char s[100010];

int main()
{
    scanf("%s", s);
    for (int i = 0; s[i] != '\0'; i++) sa.str[sa.len++] = s[i];
    int mid = sa.len;
    sa.str[sa.len++] = 1;
    scanf("%s", s);
    for (int i = 0; s[i] != '\0'; i++) sa.str[sa.len++] = s[i];
    sa.str[sa.len] = '\0';

    sa.build();
    sa.calc_hgt();

    int ans = 0;
    for (int i = 1; i < sa.len; i++)
        if (sa.hgt[i] > ans && ((sa.suff[i - 1] < mid && sa.suff[i] > mid) || (sa.suff[i - 1] > mid && sa.suff[i] < mid)))
            ans = sa.hgt[i];

    printf("%d\n", ans);

    return 0;
}
