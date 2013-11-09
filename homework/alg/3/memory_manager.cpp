#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using std::cin;
using std::cout;
using std::cerr;
using std::vector;
using std::list;
using std::swap;
using std::endl;

struct TInterval
{
    TInterval(int freeMem, int indx, int startPosition)
        : free(freeMem), index(indx), start(startPosition)
    {}

    int free;
    int index;
    int start;
    int requestId;
    bool operator>(const TInterval& interval) const
    {
        if (this->free == interval.free) {
            return this->start < interval.start;
        }
        return this->free > interval.free;
    }
};

typedef list<TInterval>::iterator IntervalIterator;

struct TRequest
{
    bool rejected;
    IntervalIterator it;
};

class MemoryManager
{
public:
    explicit MemoryManager(int memorySize)
    {
        intervals.push_front(TInterval(memorySize, 0, 1));
        heap.push_back(intervals.begin());
    }

    void revoke(int requestId)
    {
        TRequest nreq;
        nreq.rejected = true;

        TRequest req = requests[requestId];
        if (!req.rejected) {
            req.rejected = true;
            int sz = -(req.it->free);
            int st = req.it->start;
            bool inserted = false;

            IntervalIterator it = intervals.erase(req.it);

            if (it == intervals.end()) {
                if (it != intervals.begin()) {
                    --it;
                    if (it->free >= 0) {
                        it->free += sz;
                        update(it);
                        inserted = true;
                    } else {
                        ++it;
                    }
                }
            } else {
                if (it == intervals.begin()) {
                    if (it->free >= 0) {
                        it->free += sz;
                        it->start = st;
                        update(it);
                        inserted = true;
                    }
                } else {
                    IntervalIterator prev = it;
                    --prev;
                    if (it->free >= 0 && prev->free >= 0) {
                        prev->free += it->free + sz;
                        update(prev);
                        remove(it);
                        inserted = true;
                    } else {
                        if (prev->free >= 0) {
                            prev->free += sz;
                            update(prev);
                            inserted = true;
                        }

                        if (it->free >= 0) {
                            it->free += sz;
                            it->start = st;
                            update(it);
                            inserted = true;
                        }
                    }
                }
            }

            if (!inserted) {
                intervals.insert(it, TInterval(sz, heap.size(), st));
                --it;
                insertInterval(it);
            }
        }

        requests.push_back(nreq);
    }

    void addInterval(int size)
    {
        IntervalIterator max = popInterval();
        TRequest req;
        if (size > max->free) {
            req.rejected = true;
            insertInterval(max);
        } else {
            req.rejected = false;
            intervals.insert(max, TInterval(-size, heap.size(), max->start)); // must be carefull;
            max->free -= size;
            max->start += size;
            // may be if free != 0
            insertInterval(max);
            max--;
            req.it = max;
        }

        if (req.rejected) {
            cout << -1;
        } else {
            cout << req.it->start;
        }
        cout << endl;

        requests.push_back(req);
    }

    void printHeap()
    {
        for (int i = 0; i < heap.size(); ++i) {
            cerr << "free: " << heap[i]->free << ", index: " << heap[i]->index;
            cerr << endl;
        }
        cerr << "======== ========" << endl;
    }

    void doOperation(int op)
    {
        if (op >= 0)
            addInterval(op);
        else
            revoke(-op - 1);
    }

private:
    void update(IntervalIterator it)
    {
        shiftUp(it);
        shiftDown(it);
    }

    void remove(IntervalIterator it)
    {
        IntervalIterator ft = heap.back();
        swp(it, ft);
        heap.pop_back();
        update(ft);
    }

    void insertInterval(IntervalIterator it)
    {
        heap.push_back(it);
        update(it);
    }

    IntervalIterator popInterval()
    {
        IntervalIterator ret = heap.front();
        swp(heap.front(), heap.back());
        IntervalIterator ft = heap.front();
        heap.pop_back();
        update(ft);


        return ret;
    }

    IntervalIterator parent(IntervalIterator it)
    {
        return heap[(it->index - 1) / 2];
    }

    IntervalIterator child(IntervalIterator it, bool right)
    {
        int ind = it->index * 2 + right + 1;
        if (ind < heap.size()) {
            return heap[ind];
        } else {
            return it;
        }
    }

    void swp(IntervalIterator ft, IntervalIterator sd)
    {
        swap(ft->index, sd->index);
        swap(heap[ft->index], heap[sd->index]);
    }

    void shiftUp(IntervalIterator it)
    {
        // cerr << endl << "shift up:" << endl << endl;
        // printHeap();
        while (*it > *parent(it)) {
            swp(it, parent(it));
            // printHeap();
            // cerr << it->index << " : " << parent(it)->index << endl;
        }
    }

    void shiftDown(IntervalIterator it)
    {
        IntervalIterator lc, rc;
        lc = child(it, false);
        rc = child(it, true);

        if (*rc > *lc)
            lc = rc;
        while (*lc > *it) {
            swp(it, lc);
        }
    }

    vector<TRequest> requests;
    list<TInterval> intervals;
    vector<IntervalIterator> heap;
};

int main(int argc, char *argv[])
{
    int maxMemory, operationNumber;
    cin >> maxMemory >> operationNumber;
    MemoryManager mm(maxMemory);

    for (int i = 0; i < operationNumber; ++i) {
        int op;
        cin >> op;
        mm.doOperation(op);
    }
    return 0;
}
