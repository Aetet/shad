#include <iostream> 
#include <vector> 
#include <algorithm> 

using std::vector;
using std::cin;
using std::cout;
using std::sort;

int main(int argc, char *argv[])
{
    int numOfSeq, numOfelem;
    cin >> numOfSeq >> numOfelem;
    vector<int> elems;

    elems.resize(numOfSeq * numOfelem);

    for (int i = 0; i < elems.size(); ++i) {
        cin >> elems[i];
    }

    sort(elems.begin(), elems.end());
    
    for (int i = 0; i < elems.size(); ++i) {
        cout << elems[i] << " ";
    }
    return 0;
}
