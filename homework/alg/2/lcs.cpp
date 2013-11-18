#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::cin;
using std::cerr;
using std::vector;
using std::max;

template<class T>
class LCS
{
public:
    explicit LCS(const vector<T>& first, const vector<T>& second)
    {
        lcsLength.resize(first.size() + 1);
        for (int i = 0; i < lcsLength.size(); ++i) {
            lcsLength[i].resize(second.size() + 1, 0);
        }

        for (int i = 0; i < first.size(); ++i) {
            for (int j = 0; j < second.size(); ++j) {
                if (first[i] == second[j]) {
                    lcsLength[i + 1][j + 1] = lcsLength[i][j] + 1;
                } else {
                    lcsLength[i + 1][j + 1] = max(lcsLength[i][j + 1]
                                                , lcsLength[i + 1][j]);
                }
            }
        }
    }

    int getLength() const
    {
        return lcsLength.back().back();
    }

private:
    vector<vector<int> > lcsLength;
};

int main(int argc, char *argv[])
{
    int length;
    vector<int> first, second;

    cin >> length;
    first.resize(length);
    for (int i = 0; i < length; ++i) {
        cin >> first[i];
    }

    cin >> length;
    second.resize(length);
    for (int i = 0; i < length; ++i) {
        cin >> second[i];
    }

    LCS<int> lcs(first, second);

    cout << lcs.getLength();

    return 0;
}
