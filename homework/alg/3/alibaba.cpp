#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <climits>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::min;
using std::vector;
using std::abs;
using std::sort;
using std::pair;

const int inf = 2000000000;

class TP
{
public:
    bool isUpdated()
    {
        return updated;
    }

    TP()
        : value(inf), updated(false)
    {}

    int& operator()()
    {
        updated = true;
        return value;
    }

private:
    int value;
    bool updated;
};

class TPair
{
public:
    TP& operator[](int index)
    {
        return time[index];
    }

private:
    TP time[2];
};

struct TCoin
{
    int position;
    int time;
    TCoin(int _position, int _time)
        : position(_position), time(_time)
    {}

    bool operator<(const TCoin& right) const
    {
        if (this->position == right.position)
            return this->time < right.time;
        return this->position < right.position;
    }
};

class TSolver
{
public:
    explicit TSolver(int coinsNum)
        : time(coinsNum, vector<TPair>(coinsNum))
    {}

    void printAnswer()
    {}

    void sortCoins()
    {
        sort(coins.begin(), coins.end());
    }

    int distance(int first, int last)
    {
        return abs(coins[first].position - coins[last].position);
    }

    int getAnswer(int first, int last, bool stopedInRight)
    {
        if (first == last)
            return 0;

        if (time[first][last][stopedInRight].isUpdated())
            return time[first][last][stopedInRight]();

        if (!stopedInRight) {
            int LeftEndsLeft  = getAnswer(first + 1, last, false)
                              + distance(first, first + 1);
            int LeftEndsRight = getAnswer(first + 1, last, true)
                              + distance(first, last);

            if (coins[first].time < LeftEndsLeft)
                LeftEndsLeft = inf;

            if (coins[first].time < LeftEndsRight)
                LeftEndsRight = inf;

            int leftmin = min(LeftEndsLeft, LeftEndsRight);
            time[first][last][stopedInRight]() = leftmin;
            return leftmin;
        } else {
            int RightEndsRight = getAnswer(first, last - 1, true)
                               + distance(last, last - 1);
            int RightEndsLeft  = getAnswer(first, last - 1, false)
                               + distance(first, last);

            if (coins[last].time < RightEndsLeft)
                RightEndsLeft = inf;

            if (coins[last].time < RightEndsRight)
                RightEndsRight = inf;

            int rightmin = min(RightEndsLeft, RightEndsRight);
            time[first][last][stopedInRight]() = rightmin;
            return rightmin;
        }
    }

    void addCoin(const TCoin& newCoin)
    {
        coins.push_back(newCoin);
    }

private:
    vector<TCoin> coins;
    vector<vector<TPair> > time;
};

int main(int argc, char *argv[])
{
    int coinsNum;
    cin >> coinsNum;

    TSolver solver(coinsNum);
    for (int i = 0; i < coinsNum; ++i) {
        int tcoord, ttime;
        cin >> tcoord >> ttime;
        solver.addCoin(TCoin(tcoord, ttime));
    }

    solver.sortCoins();

    int ans = inf;
    ans = min(ans, solver.getAnswer(0, coinsNum - 1, false));
    ans = min(ans, solver.getAnswer(0, coinsNum - 1, true));

    if (ans >= inf)
        cout << "No solution" << endl;
    else
        cout << ans;
    return 0;
}
