#include <iostream>
#include <deque> 
#include <vector>
#include <cstdio>

using std::deque;
using std::cin;
using std::cout;
using std::vector;
using std::endl;

class RangeMaximum
{
public:

    void pop(int elem)
    {
        if (!elements.empty()
        &&  elements.front() == elem) {
            elements.pop_front();
        }
    }

    void push(int elem)
    {
        while (!elements.empty()
            && elements.back() < elem) {
            elements.pop_back();
        }

        elements.push_back(elem);
    }

    int getMaximum()
    {
        return elements.front();
    }

private:
    deque<int> elements;
};

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    //freopen("input.txt", "r", stdin);

    int m, n, t, l, r;
    char c;
    vector<int> sequence;
    RangeMaximum rm;

    sequence.reserve(100000);
    l = r = 0;

    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> t;
        sequence.push_back(t);
    }
    rm.push(sequence[0]);

    cin >> m;
    for (int i = 0; i < m; ++i) {
        cin >> c;
        if (c == 'R') {
            ++r;
            rm.push(sequence[r]);
        } else {
            rm.pop(sequence[l]);
            ++l;
        }
        cout << rm.getMaximum() << " ";
    }

    return 0;
}
