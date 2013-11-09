#include <iostream>
#include <vector>
#include <iterator>
#include <queue>
#include <functional>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::swap;
using std::less;
using std::endl;
using std::cerr;

struct TNode
{
    int key;
    int index[2];

    TNode()
    {
        index[0] = -1;
        index[1] = -1;
    }

    explicit TNode(int _key)
    {
        key = _key;
        index[0] = -1;
        index[1] = -1;
    }

    bool operator>(const TNode& tn) const
    {
        return this->key > tn.key;
    }

    bool operator<(const TNode& tn) const
    {
        return this->key < tn.key;
    }

    bool inMinHeap()
    {
        return index[0] != -1;
    }

    bool inMaxHeap()
    {
        return index[1] != -1;
    }
};

typedef TNode* tp;

TNode a;

template<class BDIT, int ind = 0>
class IndexGetter
{
public:
    IndexGetter()
    {}

    int& operator()(BDIT it)
    {
        int index = ind;
        if (0 <= ind && ind <= 1) {
            index = ind;
        }
        return it->index[index];
    }

private:
};

TNode invalid;

template <class T
        , class IndexGetter
        , class Compare
        = std::less <typename std::iterator_traits <T>::value_type> >
class IndexHeap
{
public:
    IndexHeap()
    {}

    bool empty()
    {
        return heap.empty();
    }

    void remove(T it)
    {
        if (!empty()) {
            int tind = ig(it);
            if (tind == size() - 1) {
                ig(heap.back()) = -1;
                heap.pop_back();
                return;
            }
            swp(tind, size() - 1);
            ig(heap.back()) = -1;
            heap.pop_back();

            if (!empty()) {
                update_heap(heap[tind]);
            }
        }
    }

    size_t size()
    {
        return heap.size();
    }

    T top()
    {
        if (!empty())
            return heap[0];
        return &invalid;
    }

    T pop()
    {
        if (!empty()) {
            T ret = top();
            remove(ret);
            return ret;
        }
        return &invalid;
    }

    void insert(T elem)
    {
        if (elem == &invalid)
            return;

        if (ig(elem) != -1)
            return;
        ig(elem) = size();
        heap.push_back(elem);
        update_heap(elem);
    }

    void swp(int ftind, int sdind)
    {
        swap(ig(heap[ftind]), ig(heap[sdind]));
        swap(heap[ftind], heap[sdind]);
    }

    void print()
    {
        std::cerr << "****************" << endl;
        for (int i = 0; i < size(); ++i) {
            std::cerr << "heap[" << i << "] : " << heap[i]->key << ", "
                      << ig(heap[i]) << std::endl;
        }
        std::cerr << "****************" << endl << endl;
    }

    void update_heap(T it)
    {
        while (shiftUp(it)) {}
        while (shiftDown(it)) {}
    }

    bool shiftUp(T it)
    {
        int tind, pind;
        tind = ig(it);
        pind = (tind - 1) / 2;
        // cerr << tind << " : " << pind << endl;
        if (comp(*(heap[tind]), *(heap[pind]))) {
            swp(tind, pind);
            return true;
        }
        return false;
    }

    bool shiftDown(T it)
    {
        int leftChild, rightChild, largestChild;
        int index = ig(it);

        leftChild = index * 2 + 1;
        rightChild = index * 2 + 2;
        largestChild = index;

        if (leftChild < size()
         && comp(*(heap[leftChild]), *(heap[largestChild]))) {
            largestChild = leftChild;
        }

        if (rightChild < size()
         && comp(*(heap[rightChild]), *(heap[largestChild]))) {
            largestChild = rightChild;
        }

        if (largestChild != index) {
            swp(index, largestChild);
            return true;
        }

        return false;
    }

private:
    IndexGetter ig;
    vector<T> heap;
    Compare comp;
};

int main(int argc, char *argv[])
{
    int elemNum, comNum, statNum;
    invalid.key = -1;
    vector<TNode> nodes;

    IndexHeap<tp, IndexGetter<tp, 0>, std::less<TNode> > min_heap;
    IndexHeap<tp, IndexGetter<tp, 1>, std::greater<TNode> > max_heap;

    cin >> elemNum >> comNum >> statNum;

    for (int i = 0; i < elemNum; ++i) {
        int telem;
        cin >> telem;
        nodes.push_back(TNode(telem));
    }

    int left = 0, right = 0;
    max_heap.insert(&nodes[0]);

    for (int ind = 0; ind < comNum; ++ind) {
        char com;
        cin >> com;

        if (com == 'R') {
            ++right;
            min_heap.insert(&nodes[right]);
        }

        /*
         * if (ind == 3) {
         *     cerr << "debug: " << endl;
         *     cerr << left << " " << right << endl;
         *     cerr << nodes[left].index[1] << endl;
         *     max_heap.print();
         * }
         */

        if (com == 'L') {
            if (nodes[left].inMaxHeap()) {
                max_heap.remove(&nodes[left]);
            } else {
                min_heap.remove(&nodes[left]);
            }
            ++left;
        }

        // cerr << max_heap.size() << " : " << min_heap.size() << endl;
        while (max_heap.size() < statNum && !min_heap.empty()) {
            tp it = min_heap.pop();
            // cerr << it->key << endl;
            max_heap.insert(it);
        }

        while (!max_heap.empty()
            && !min_heap.empty()
            && *max_heap.top() > *min_heap.top()) {
            min_heap.insert(max_heap.pop());
            max_heap.insert(min_heap.pop());
        }

        if (max_heap.size() < statNum) {
            cout << -1 << endl;
        } else {
            cout << max_heap.top()->key << endl;
        }
    }

    return 0;
}
