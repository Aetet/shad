#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <iterator>
#include <cstdlib>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::list;
using std::swap;
using std::cerr;
using std::greater;

struct TSegment
{
    TSegment()
        : index(-1)
    {}

    int segmentSize;
    int firstCell;
    int index;
    bool free;

    bool operator<(const TSegment& rhl) const
    {
        if (this->segmentSize == rhl.segmentSize)
            return this->firstCell < rhl.firstCell;
        return this->segmentSize < rhl.segmentSize;
    }

    bool operator>(const TSegment& rhl) const
    {
        if (this->segmentSize == rhl.segmentSize)
            return this->firstCell > rhl.firstCell;
        return this->segmentSize > rhl.segmentSize;
    }
};

typedef list <TSegment>::iterator segmentIterator;

template<class BDIT>
class IndexGetter
{
public:
    IndexGetter()
    {}

    int& operator()(BDIT it)
    {
        return it->index;
    }

private:
};

template <class T
        , class IndexGetter
        = IndexGetter<T>
        , class Compare
        = std::greater <typename std::iterator_traits <T>::value_type> >
class IndexHeap
{
public:
    IndexHeap()
    {}

    bool empty() const
    {
        return heap.empty();
    }

    void remove(T it)
    {
        if (!empty()) {
            int tind = index(it);
            if (tind == -1)
                return;

            if (tind == size() - 1) { // spike
                index(heap.back()) = -1;
                heap.pop_back();
                return;
            }

            swp(tind, size() - 1);
            index(heap.back()) = -1;
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
        return heap.front();
    }

    void pop()
    {
        if (!empty()) {
            T ret = top();
            remove(ret);
        }
    }

    void insert(T elem)
    {
        index(elem) = size();
        heap.push_back(elem);
        update_heap(elem);
    }

    void swp(int ftind, int sdind)
    {
        swap(index(heap[ftind]), index(heap[sdind]));
        swap(heap[ftind], heap[sdind]);
    }

    void print()
    {
        std::cerr << "****************" << endl;
        for (int i = 0; i < size(); ++i) {
            std::cerr << "heap[" << i << "] : " << heap[i]->segmentSize << ", "
                      << index(heap[i]) << std::endl;
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
        tind = index(it);
        pind = (tind - 1) / 2;

        if (compare(*(heap[tind]), *(heap[pind]))) {
            swp(tind, pind);
            return true;
        }

        return false;
    }

    bool shiftDown(T it)
    {
        int leftChild, rightChild, largestChild;
        int itIndex = index(it);

        leftChild = itIndex * 2 + 1;
        rightChild = itIndex * 2 + 2;
        largestChild = itIndex;

        if (leftChild < size()
         && compare(*(heap[leftChild]), *(heap[largestChild]))) {
            largestChild = leftChild;
        }

        if (rightChild < size()
         && compare(*(heap[rightChild]), *(heap[largestChild]))) {
            largestChild = rightChild;
        }

        if (largestChild != itIndex) {
            swp(itIndex, largestChild);
            return true;
        }

        return false;
    }

private:
    IndexGetter index;
    vector<T> heap;
    Compare compare;
};

struct TRequest
{
    TRequest()
        : rejected(false)
    {}
    bool rejected;
    segmentIterator segment;
};

class TMemoryManager
{
public:
    explicit TMemoryManager(int memorySize)
    {
        TSegment allMemory;
        allMemory.firstCell = 1;
        allMemory.segmentSize = memorySize;
        allMemory.free = true;

        memorySegments.push_back(allMemory);
        freeMemory.insert(memorySegments.begin());
    }

    void revoke(int requestNumber)
    {
        --requestNumber;

        TRequest request;
        request.rejected = true;
        requests.push_back(request);

        if (requests[requestNumber].rejected)
            return;
        requests[requestNumber].rejected = true;

        segmentIterator segment = requests[requestNumber].segment;
        segment->free = true;
        segmentIterator next = segment;
        segmentIterator prev = segment;
        ++next;
        tryMerge(segment, next);
        --prev;

        bool successMerge = tryMerge(prev, segment);
        if (successMerge) {
            freeMemory.insert(prev);
        } else {
            freeMemory.insert(segment);
        }
    }

    int allocate(int memorySize)
    {
        if (freeMemory.empty()
         || freeMemory.top()->segmentSize < memorySize) {
            TRequest request;
            request.rejected = true;
            requests.push_back(request);
            return -1;
        }

        TRequest request;

        segmentIterator maxSegment = freeMemory.top();
        freeMemory.pop();
        int firstFreeCell = maxSegment->firstCell;

        if (maxSegment->segmentSize > memorySize) {
            TSegment newMemSegment;
            newMemSegment = shrink(maxSegment, memorySize);
            memorySegments.insert(maxSegment, newMemSegment);
            freeMemory.insert(maxSegment);
            --maxSegment;
            request.segment = maxSegment;
        } else {
            maxSegment->free = false;
            request.segment = maxSegment;
        }

        requests.push_back(request);
        // freeMemory.print();
        return firstFreeCell;
    }

private:
    bool tryMerge(segmentIterator& ft, segmentIterator& sd)
    {
        if (sd == memorySegments.end() || sd == memorySegments.begin())
            return false;

        if (ft->free && sd->free) {
            freeMemory.remove(ft);
            freeMemory.remove(sd);
            ft->segmentSize += sd->segmentSize;
            memorySegments.erase(sd);
            freeMemory.insert(ft);
            return true;
        }

        return false;
    }

    TSegment shrink(segmentIterator segment, int memorySize)
    {
        TSegment newSegment;
        newSegment.segmentSize = memorySize;
        newSegment.firstCell = segment->firstCell;
        newSegment.free = false;

        segment->segmentSize -= memorySize;
        segment->firstCell += memorySize;

        return newSegment;
    }

    vector<TRequest> requests;
    list<TSegment> memorySegments;
    IndexHeap <segmentIterator> freeMemory;
};

int main(int argc, char *argv[])
{
    int memorySize, requestNumber;

    cin >> memorySize >> requestNumber;
    TMemoryManager mm(memorySize);

    for (int i = 0; i < requestNumber; ++i) {
        int request;
        cin >> request;
        if (request > 0) {
            cout << mm.allocate(request) << endl;
        } else {
            mm.revoke(-request);
        }
    }

    return 0;
}
