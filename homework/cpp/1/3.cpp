#include <iostream> 
#include <vector>
#include <cstdlib>
#include <algorithm>

using std::cerr;
using std::cout;
using std::vector;

template<typename T>
class sequence
{
public:
    sequence(T _begin, T _end) :
        begin(_begin), end(_end)
    {}

    void rotate(int k)
    {
        size_t sequenceLength = end - begin;
        int f = 1;

        if (k < 0)
            f = -1;
        k %= sequenceLength;
        k = f > 0 ? sequenceLength - k : k;

        reverse(begin, begin + k);
        reverse(begin + k, end);
        reverse(begin, end);
    }

private:
    T begin;
    T end;
};

int main(int argc, char *argv[])
{
    vector<int> v;
    for (int i = 1; i <= 6; ++i)
        v.push_back(i);

    for (int i = 0; i < v.size(); ++i)
        cout << v[i] << " ";
    cout << std::endl;

    sequence<std::vector<int>::iterator> seq(v.begin(), v.end());
    seq.rotate(-2);

    for (int i = 0; i < v.size(); ++i)
        cout << v[i] << " ";
    cout << std::endl;

    return 0;
}
