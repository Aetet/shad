#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::cerr;

class cube
{
public:
    explicit cube (int height
        , int width
        , int length)
        : _height(height)
        , _width(width)
        , _length(length)
    {}

    bool operator<(const cube& cb) const
    {
        return (this->_height < cb._height
             && this->_width < cb._width
             && this->_length < cb._length);
    }

private:
    int _height, _width, _length;
};

template<class T>
class LargestIncreasingSubsequence
{
public:
    explicit LargestIncreasingSubsequence(const vector<T>& initVector)
                               : begin(initVector.begin())
                               , end(initVector.end())
    {}

    int getLength()
    {
        int sequenceSize = end - begin;
        typename vector<T>::const_iterator curr, it;

        subseqLength.resize(sequenceSize, 1);

        for (curr = begin; curr != end; ++curr)
            for (it = begin; it < curr; ++it)
                if ((*it) < (*curr)
                 && subseqLength[curr - begin] <= subseqLength[it - begin]) {
                    subseqLength[curr - begin] = subseqLength[it - begin] + 1;
                }

        int maximumIndex = 0;
        for (int i = 0; i < subseqLength.size(); ++i) {
            if (subseqLength[i] > subseqLength[maximumIndex]) {
                maximumIndex = i;
            }
        }

        return subseqLength[maximumIndex];
    }

private:
    typename vector<T>::const_iterator begin, end;
    vector<int> subseqLength;
};

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);

    vector<cube> cubes;
    int numberOfCubes;
    int height,
        width,
        length;

    cubes.reserve(1000);
    cin >> numberOfCubes;
    for (int i = 0; i < numberOfCubes; ++i) {
        cin >> height >> width >> length;
        cube cb(height
              , width
              , length);
        cubes.push_back(cb);
    }

    LargestIncreasingSubsequence<cube> lis(cubes);
    cout << lis.getLength();

    return 0;
}
