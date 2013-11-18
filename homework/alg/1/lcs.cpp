#include <iostream>
#include <vector>
#include <cstdlib>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::distance;

class LargestChangingSubsequence
{
public:
    explicit LargestChangingSubsequence(const vector<int>& initVector)
        : sequence(&initVector)
    {
        elementsInSubsequence[0].resize(initVector.size(), 1);
        elementsInSubsequence[1].resize(initVector.size(), 1);
        parent[0].resize(initVector.size(), -1);
        parent[1].resize(initVector.size(), -1);
    }

    vector<int> getLargestChangingSubsequence()
    {
        generateSubsequence();
        vector<int> ans;
        int maxIndex[2] = {getIndexOfMax(elementsInSubsequence[0])
                         , getIndexOfMax(elementsInSubsequence[1])};

        int rstartIndex = 0;

        if (elementsInSubsequence[0][maxIndex[0]]
         <  elementsInSubsequence[1][maxIndex[1]])
            rstartIndex = 1;

        int curr = maxIndex[rstartIndex];
        int subseqLength 
          = elementsInSubsequence[rstartIndex][maxIndex[rstartIndex]];

        /*
         * cerr << "maxIndexs: " << maxIndex[0] << " " << maxIndex[1] << endl;
         * cerr << "subseqLen: " << subseqLength << endl;
         * cerr << curr << endl;
         */

        for (int i = 0; i < subseqLength; ++i, rstartIndex = 1 - rstartIndex) {
            ans.push_back((*sequence)[curr]);
            curr = parent[rstartIndex][curr];
        }

        return ans;
    }

private:
    typedef vector<int>::const_reverse_iterator RCIt;

    int getIndex(const RCIt& it)
    {
        return abs(distance(sequence->rend(), it) + 1);
    }

    void generateSubsequence()
    {
        RCIt rit, rjt;
        for (rit = sequence->rbegin(); rit != sequence->rend(); ++rit) {
            for (rjt = sequence->rbegin(); rjt != rit; ++rjt) {
                if (*rjt < *rit
                 && elementsInSubsequence[1][getIndex(rit)]
                 <= elementsInSubsequence[0][getIndex(rjt)] + 1) {
                    elementsInSubsequence[1][getIndex(rit)]
                  = elementsInSubsequence[0][getIndex(rjt)] + 1;
                    parent[1][getIndex(rit)] = getIndex(rjt);
                }

                if (*rjt > *rit
                 && elementsInSubsequence[0][getIndex(rit)]
                 <= elementsInSubsequence[1][getIndex(rjt)] + 1) {
                    elementsInSubsequence[0][getIndex(rit)]
                  = elementsInSubsequence[1][getIndex(rjt)] + 1;
                    parent[0][getIndex(rit)] = getIndex(rjt);
                }
            }
        }
    }

    int getIndexOfMax(const vector<int>& vec)
    {
        int ret = 0;

        for (int i = 0; i < vec.size(); ++i) {
            if (vec[ret] < vec[i]) {
                ret = i;
            }
        }

        return ret;
    }

    const vector<int> *sequence;
    vector<int> elementsInSubsequence[2];
    vector<int> parent[2];
};

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);

    vector<int> inputVector;
    int numberOfElements;

    inputVector.reserve(1000);

    cin >> numberOfElements;
    inputVector.resize(numberOfElements);
    for (int i = 0; i < numberOfElements; ++i) {
        cin >> inputVector[i];
    }
    
    LargestChangingSubsequence lcs(inputVector);

    vector<int> ans(lcs.getLargestChangingSubsequence());
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i] << " ";
    }
    return 0;
}
