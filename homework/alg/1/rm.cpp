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

    int getMaximum() const
    {
        return elements.front();
    }

private:
    deque<int> elements;
};

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    // freopen("input.txt", "r", stdin);

    int numberOfElements
      , elem
      , left
      , right;
    char command;
    vector<int> sequence;
    RangeMaximum rm;

    sequence.reserve(100000);
    left = right = 0;

    cin >> numberOfElements;
    for (int i = 0; i < numberOfElements; ++i) {
        cin >> elem;
        sequence.push_back(elem);
    }
    rm.push(sequence[0]);

    int numberOfCommands;
    cin >> numberOfCommands;
    for (int i = 0; i < numberOfCommands; ++i) {
        cin >> command;
        if (command == 'R') {
            ++right;
            rm.push(sequence[right]);
        } else {
            rm.pop(sequence[left]);
            ++left;
        }
        cout << rm.getMaximum() << " ";
    }

    return 0;
}
