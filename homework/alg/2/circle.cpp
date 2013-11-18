#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using std::cin;
using std::cout;
using std::cerr;
using std::vector;
using std::abs;
using std::max;

const double eps = 1e-7;

struct TPoint
{
    TPoint(double _x, double _y)
        : cx(_x), cy(_y)
    {}
    double cx, cy;
};

struct TLinePoint
{
    double cx;
    bool isOpen;
    TLinePoint()
    {}

    TLinePoint(double _x, bool _isOpen)
        : cx(_x), isOpen(_isOpen)
    {}

    bool operator<(const TLinePoint& right) const
    {
        if (right.cx - this->cx > eps)
            return true;
        if (abs(right.cx - this->cx) < eps)
            return this->isOpen;
        return false;
    }
};

int getIntersectionNumber(const double radius, const vector<TPoint>& points)
{
    const double rsq = radius * radius;
    vector<TLinePoint> pts;
    // pts.reserve(2 * points.size());

    for (int i = 0; i < points.size(); ++i) {
        double psq = points[i].cy * points[i].cy;
        if (rsq - psq > 0) {
            double delta = sqrt(rsq - psq);
            pts.push_back(TLinePoint(points[i].cx - delta, 1));
            pts.push_back(TLinePoint(points[i].cx + delta, 0));
        }
    }

    sort(pts.begin(), pts.end());
    int maxInt = 0, countInt = 0;
    for (int i = 0; i < pts.size(); ++i) {
        if (pts[i].isOpen)
            ++countInt;
        else
            --countInt;
        maxInt = max(maxInt, countInt);
    }

    return maxInt;
}



int main(int argc, char *argv[])
{
    int elemNum;
    vector<TPoint> pts;
    int minK;
    double cx, cy;

    cin >> elemNum;
    cin >> minK;

    for (int i = 0; i < elemNum; ++i) {
        cin >> cx >> cy;
        pts.push_back(TPoint(cx, cy));
    }

    double left = 0.0, right = 4000;
    while (right - left >= eps) {
        double mid = (left + right) / 2.0;
        int intNum = getIntersectionNumber(mid, pts);
        if (intNum >= minK) {
            right = mid;
        } else {
            left = mid;
        }
    }

    cout.precision(12);
    cout << left;
    return 0;
}
