/*
 *  SRC: POJ 3481
 * PROB: Double Queue
 * ALGO: Splay
 * DATE: Sep 04, 2011 
 * COMP: g++
 *
 * Created by Leewings Ac
 */

#include <cstdio>
#include <cstring>
#include <utility>

using std::pair;

class Splay {
    private:
        const static int BUF_SIZE = 1000010;
        int size;

        struct Node {
            int key;
            int id;
            Node* l;
            Node* r;
        };

        int buf_cnt;
        Node buf[BUF_SIZE];
        Node* root;

        Node* get_max(Node* x) const
        {
            while (x->r) x = x->r;
            return x;
        }

        Node* get_min(Node* x) const
        {
            while (x->l) x = x->l;
            return x;
        }

        Node* left_rotate(Node* x)
        {
            Node* y = x->r;
            x->r = y->l;
            y->l = x;

            return y;
        }

        Node* right_rotate(Node* x)
        {
            Node* y = x->l;
            x->l = y->r;
            y->r = x;

            return y;
        }

        Node* splay(Node* x, int key)
        {
            if (!x) return x;

            Node* y = new Node;
            y->l = y->r = 0;
            Node* larger = y;
            Node* smaller = y;
            
            while (key != x->key) {
                if (key < x->key) {
                    if (x->l && key < x->l->key) x = right_rotate(x);
                    if (!x->l) break;
                    larger->l = x;
                    larger = x;
                    x = x->l;
                }
                else {  // key > x->key
                    if (x->r && key > x->r->key) x = left_rotate(x);
                    if (!x->r) break;
                    smaller->r = x;
                    smaller = x;
                    x = x->r;
                }
            }

            larger->l = x->r;
            smaller->r = x->l;
            x->l = y->r;
            x->r = y->l;

            delete y;

            return x;
        }

        Node* _insert(Node* x, int key, int id)
        {
            if (x) x = splay(x, key);

            size++;
            Node* y = &buf[buf_cnt++];
            y->key = key;
            y->id = id;
            y->l = y->r = 0;

            if (!x) return y;

            if (key < x->key) {
                y->l = x->l;
                y->r = x;
                x->l = 0;
            }
            else {
                y->r = x->r;
                y->l = x;
                x->r = 0;
            }

            return y;
        }

        Node* _erase(Node* x, int key)
        {
            if (!x) return 0;

            x = splay(x, key);
            if (key == x->key) {
                size--;
                Node* y;
                if (!x->l) y = x->r;
                else {
                    y = splay(x->l, key);
                    y->r = x->r;
                }

                return y;
            }
            return x;
        }

    public:
        Splay()
        {
            reset();
        }

        void reset()
        {
            size = buf_cnt = 0;
            memset(buf, 0, sizeof(buf));
            root = 0;
        }

        bool empty()
        {
            return !size;
        }

        void insert(int key, int id)
        {
            root = _insert(root, key, id);
        }

        void erase(int key)
        {
            root = _erase(root, key);
        }

        pair<int, int> find_max() const
        {
            Node* x = get_max(root);
            return pair<int, int>(x->key, x->id);
        }

        pair<int, int> find_min() const
        {
            Node* x = get_min(root);
            return pair<int, int>(x->key, x->id);
        }
};

Splay splay;

int main()
{
    int order;
    while (scanf("%d", &order), order) {
        switch(order) {
            case 0: return 0;
            case 1: {
                        int key, id;
                        scanf("%d%d", &id, &key);
                        splay.insert(key, id);
                        break;
                    }
            case 2: {
                        if (splay.empty()) puts("0");
                        else {
                            pair<int, int> ans = splay.find_max();
                            printf("%d\n", ans.second);
                            splay.erase(ans.first);
                        }
                        break;
                    }
            case 3: {
                        if (splay.empty()) puts("0");
                        else {
                            pair<int, int> ans = splay.find_min();
                            printf("%d\n", ans.second);
                            splay.erase(ans.first);
                        }
                    }
        }
    }

    return 0;
}
