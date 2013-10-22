#include <iostream>
#include <algorithm> 
#include <vector> 
#include <utility>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;

typedef pair<unsigned int, int> player;

template<class T>
void mySort(T begin, T end)
{
    std::make_heap(begin, end);
    while (begin != end) {
        std::pop_heap(begin, end);
        --end;
    }
}

long long getSum(const vector<long long>& effectivitySum, int first, int last)
{
    return effectivitySum[last] - (first > 0 ? effectivitySum[first - 1] : 0);
}

int getMinFirstIndex(const vector<player>& players
                   , int lastIndex)
{
    int ret = std::lower_bound(players.begin()
                             , players.end()
                             , make_pair(players[lastIndex].first / 2
                                       + players[lastIndex].first % 2, 0))
            - players.begin();
    return ret;
}

void printPlayers(const vector<player>& players, int first, int last)
{
    vector<int> answer;

    for (int i = first; i <= last; ++i) {
        answer.push_back(players[i].second + 1);
    }

    mySort<vector<int>::iterator>(answer.begin(), answer.end());
    for (int i = 0; i < answer.size(); ++i) {
        cout << answer[i] << " ";
    }
}

int main(int argc, char *argv[])
{
    int playersCount;
    vector<player> players;
    vector<long long> effectivitySum;

    cin >> playersCount;
    players.resize(playersCount);
    effectivitySum.resize(playersCount);


    for (int i = 0; i < playersCount; ++i) {
        cin >> players[i].first;
        players[i].second = i;
    }

    mySort<vector<player>::iterator>(players.begin(), players.end());
    effectivitySum[0] = players[0].first;
    for (int i = 1; i < playersCount; ++i) {
        if (i > 0) {
            effectivitySum[i] = effectivitySum[i - 1] 
                              + static_cast<long long>(players[i].first);
        }
    }

    int maxf = 0, maxl = 0;
    long long maxs;
    int tfirst, tlast;
    long long tsum;

    maxs = getSum(effectivitySum, maxf, maxl);
    
    tfirst = 0;
    tlast = 1;
    tsum = players[0].first;
    while (tlast < players.size()) {
        tsum += players[tlast].first;

        while (players[tfirst].first + players[tfirst + 1].first 
            < players[tlast].first) {
            tsum -= players[tfirst].first;
            ++tfirst;
        }

        if (tsum > maxs) {
            maxf = tfirst;
            maxl = tlast;
            maxs = tsum;
        }

        ++tlast;
    }

    cerr << maxf << " : " << maxl << endl;
    cout << maxs << endl;
    printPlayers(players, maxf, maxl);

/*
 *    for (int ind = players.size() - 1; ind >= 0; --ind) {
 *        tlast = ind;
 *        tfirst = getMinFirstIndex(players, tlast);
 *
 *        tsum = getSum(effectivitySum, tfirst, tlast);
 *        if (tsum > maxs
 *         && players[tfirst].first + players[tfirst + 1].first 
 *         >= players[tlast].first) {
 *            maxf = tfirst;
 *            maxl = tlast;
 *            maxs = tsum;
 *        }
 *
 *        if (tfirst > 0)
 *            --tfirst;
 *
 *        tsum = getSum(effectivitySum, tfirst, tlast);
 *        if (tsum > maxs
 *         && players[tfirst].first + players[tfirst + 1].first
 *         >= players[tlast].first) {
 *            maxf = tfirst;
 *            maxl = tlast;
 *            maxs = tsum;
 *        }
 *    }
 */

    return 0;
}
