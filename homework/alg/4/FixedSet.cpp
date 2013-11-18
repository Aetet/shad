#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::list;

class TFixedSet
{
public:
    TFixedSet()
    {}

    void Initialize(const vector<int>& numbers)
    {
        int numberOfElements = 1;
        numbersDegree = 0;

        while (numberOfElements <= numbers.size()) {
            numberOfElements *= 2;
            ++numbersDegree;
        }

        mod = sizeof(int) * 8 - numbersDegree;
        table.resize(numberOfElements + 1);

        for (auto num : numbers) {
            if (!Contains(num)) {
                table[hash(num)].push_back(num);
            }
        }
    }

/*
 *    unsigned int hash(unsigned int number) {
 *        number = ((number >> 16) ^ number) * 0x45d9f3b;
 *        number = ((number >> 16) ^ number) * 0x45d9f3b;
 *        number = ((number >> 16) ^ number);
 *        number = ~number;
 *        return number >> mod;
 *    }
 *
 */
    unsigned int hash(unsigned int number)
    {
        // number = ~number;
        unsigned int highorder = number & 0xf8000000;    // extract high-order 5 bits from h
        // 0xf8000000 is the hexadecimal representation
        //   for the 32-bit number with the first five 
        //   bits = 1 and the other bits = 0   
        number = number << 5;                    // shift h left by 5 bits
        number = number ^ (highorder >> 27);     // move the highorder 5 bits to the low-order
        //   end and XOR into h
        // unsigned int ki = 39916799;
        // number = number ^ ki;                    // XOR h and ki
        return number >> mod;
    }

    bool Contains(int number)
    {
        unsigned int numHash = hash(number);
        if (!table[numHash].empty()) {
            for (auto element : table[numHash]) {
                if (element == number)
                    return true;
            }
        }

        return false;
    }

private:
    vector<list<int> > table;
    int numbersDegree;
    int mod;
};

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    int elementsNumber, requestsNumber;
    vector<int> initVector;

    cin >> elementsNumber;
    initVector.reserve(elementsNumber);

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

