#include <iostream>
#include <vector>
#include <list>

using std::cin;
using std::cout;
using std::cerr;
using std::vector;
using std::list;
using std::swap;
using std::endl;

struct TRequest
{
    bool rejected;
};

struct TInterval
{
    TInterval(int freeMem, int indx)
        : free(freeMem), index(indx)
    {}

    int free;
    int index;
    int start;
    int requestId;
    bool operator>(const TInterval& interval) const
    {
        if (this->free == interval.free) {
            return this->start < interval.start; //pcf
        }
        return this->free > interval.free;
    }
};

class MemoryManager
{
public:
    explicit MemoryManager(int memorySize)
    {
        intervals.push_front(TInterval(memorySize, 0));
        heap.push_back(intervals.begin());
    }

    void addInterval(int size)
    {
        intervals.push_front(TInterval(size, heap.size()));
        heap.push_back(intervals.begin());
        shiftUp(intervals.begin());
        shiftDown(intervals.begin());
    }

    void printHeap()
    {
        for (int i = 0; i < heap.size(); ++i) {
            cerr << "free: " << heap[i]->free << ", index: " << heap[i]->index;
            cerr << endl;
        }
        cerr << "======== ========" << endl;
    }

private:
    typedef list<TInterval>::iterator IntervalIterator;

    void insert(IntervalIterator it)
    {
        it->index = heap.size();
        heap.push_back(it);
    }

    IntervalIterator parent(IntervalIterator it)
    {
        return heap[it->index / 2];
    }

    IntervalIterator child(IntervalIterator it)
    {
        if (it->index * 2 < heap.size()) {
            return heap[it->index * 2];
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
        while(*child(it) > *it) {
            swp(it, child(it));
        }
    }

    vector<TRequest> requests;
    list<TInterval> intervals;
    vector<IntervalIterator> heap;
};

int main(int argc, char *argv[])
{
    MemoryManager mm(3);
    mm.addInterval(10);
    mm.addInterval(11);
    mm.addInterval(1);
    mm.addInterval(12);
    mm.addInterval(1);
    mm.addInterval(12);
    mm.printHeap();
    return 0;
}
