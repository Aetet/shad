#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::copy;
using std::ostream_iterator;

template<class RandomAccessIterator>
void mySort(RandomAccessIterator begin, RandomAccessIterator end)
{
    std::make_heap(begin, end);
    while (begin != end) {
        std::pop_heap(begin, end);
        --end;
    }
}

struct TPlayer
{
    unsigned int effectivity;
    unsigned int id;
};

bool operator<(const TPlayer& firstPlayer, const TPlayer& secondPlayer)
{
    return firstPlayer.effectivity < secondPlayer.effectivity;
}

template <class RandomAccessIterator>
class TSegment
{
public:
    explicit TSegment(RandomAccessIterator first
                    , RandomAccessIterator last)
        : first_(first)
        , last_(last)
        , sum_(0)
    {}

    RandomAccessIterator first()
    {
        return first_;
    }

    RandomAccessIterator last()
    {
        return last_;
    }

    RandomAccessIterator second()
    {
        RandomAccessIterator retit(first_);
        ++retit;
        return retit;
    }

    void shiftFirst()
    {
        ++first_;
    }

    void shiftLast()
    {
        ++last_;
    }

    long long& sum()
    {
        return sum_;
    }

private:
    RandomAccessIterator first_, last_;
    long long sum_;
};

class TTeamBuilder
{
public:
    explicit TTeamBuilder(int playersCount)
        : players(playersCount)
    {}

    friend std::istream& operator>>(std::istream& stream
                                  , TTeamBuilder& builder)
    {
        for (int i = 0; i < builder.players.size(); ++i) {
            stream >> builder.players[i].effectivity;
            builder.players[i].id = i + 1;
        }

        mySort(builder.players.begin(), builder.players.end());
        return stream;
    }

    long long getAnswer(vector<int>& perfectTeam);

private:
    vector<TPlayer> players;
};

long long TTeamBuilder::getAnswer(vector<int>& perfectTeam)
{
    typedef vector<TPlayer>::iterator TPIT;
    TSegment<TPIT> current(players.begin(), players.begin())
                                  , maximum(players.begin(), players.begin());

    current.sum() += current.first()->effectivity;
    maximum.sum() += maximum.first()->effectivity;

    current.shiftLast();
    while (current.last() != players.end()) {
        current.sum() += current.last()->effectivity;

        while (current.first()->effectivity + current.second()->effectivity
            < current.last()->effectivity) {
            current.sum() -= current.first()->effectivity;
            current.shiftFirst();
        }

        if (current.sum() > maximum.sum()) {
            maximum = current;
        }
        current.shiftLast();
    }
    maximum.shiftLast();


    perfectTeam.erase(perfectTeam.begin(), perfectTeam.end());
    for (TPIT it = maximum.first(); it != maximum.last(); ++it)
        perfectTeam.push_back(it->id);

    mySort(perfectTeam.begin(), perfectTeam.end());

    return maximum.sum();
}

std::ostream& operator<<(std::ostream& stream
                       , const vector<int> outVector)
{
    copy(outVector.begin(), outVector.end(), ostream_iterator<int>(cout, " "));
    return stream;
}

int main(int argc, char *argv[])
{
    int playersNumber;

    cin >> playersNumber;
    TTeamBuilder tb(playersNumber);
    cin >> tb;

    vector<int> ans;
    cout << tb.getAnswer(ans) << endl;
    cout << ans;

    return 0;
}
