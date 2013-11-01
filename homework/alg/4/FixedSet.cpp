#include <climits>
#include <vector>
#include <list>
#include <iostream>
#include <utility>
#include <string>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::cerr;
using std::list;
using std::abs;
using std::max;

const int MAX_NUMBER_OF_ELEMENTS = 562879;

class TFixedSet
{
public:
    TFixedSet()
        : table(MAX_NUMBER_OF_ELEMENTS)
    {}

    void Initialize(const vector<int>& numbers)
    {
        for (auto num : numbers) {
            if (!Contains(num))
                table[hash(num)].push_back(num);
        }
    }

/*
    static unsigned int hash(unsigned int number) {
        number = ((number >> 16) ^ number) * 0x45d9f3b;
        number = ((number >> 16) ^ number) * 0x45d9f3b;
        number = ((number >> 16) ^ number);
        return number >> 13;
    }
*/

    int hash(int number)
    {
        // return MurmurHash2(&number, sizeof(number), 7) % MAX_NUMBER_OF_ELEMENTS;
        return abs(number * 3) % MAX_NUMBER_OF_ELEMENTS;
    }


    bool Contains(int number)
    {
        int numHash = hash(number);
        if (!table[numHash].empty()) {
            for (auto element : table[numHash]) {
                if (element == number)
                    return true;
            }
        }

        return false;
    }

    void debugPrint()
    {
        int mx = 0;
        for (auto elem : table)
            mx = max(mx, static_cast<int>(elem.size()));
        cerr << mx;
    }

private:
    vector<list<int> > table;
};

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    int elementsNumber, requestsNumber;
    vector<int> initVector;
    cin >> elementsNumber;

    for (int i = 0; i < elementsNumber; ++i) {
        int element;
        cin >> element;
        initVector.push_back(element);
    }

    TFixedSet st;
    st.Initialize(initVector);

    cin >> requestsNumber;

    for (int i = 0; i < requestsNumber; ++i) {
        int element;
        cin >> element;
        cout << (st.Contains(element) ? "Yes" : "No") << endl;
    }

    return 0;
}
