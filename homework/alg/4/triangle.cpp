#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using std::sort;
using std::cerr;
using std::endl;

typedef unsigned long long ull;

const ull maxN = 50000000;
int cl[maxN];

int gcd(int ft, int sd)
{
    if (ft == 0)
        return sd;
    else
        return gcd(sd % ft, ft);
}

ull getTriangle(int ft, int sd, int th)
{
    int div;
    div = gcd(ft, gcd(sd, th));
    vector<int> ch;
    ch.push_back(ft / div);
    ch.push_back(sd / div);
    ch.push_back(th / div);
    sort(ch.begin(), ch.end());

    ull ans = ch[0] * 7LL + ch[1] * 11LL + ch[2] * 13LL
            + ch[1] * ch[1] * 3LL + ch[0] * ch[2] * 5LL;

    return ans % maxN;
}

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);

    for (int i = 0; i < maxN; ++i) {
        cl[i] = 0;
    }

    int tNum;
    cin >> tNum;
    for (int i = 0; i < tNum; ++i) {
        int ta, tb, tc;
        cin >> ta >> tb >> tc;
        ++cl[getTriangle(ta, tb, tc)];
    }

    int ans = 0;
    for (int i = 0; i < maxN; ++i) {
        if (cl[i] > 0)
            ++ans;
    }

    cout << ans;
    return 0;
}
