#include <iostream> 
#include <stdexcept>

using std::cout;
using std::cerr;

class gcd
{
public:
    /*
     *gcd (int _a, int _b)
     *{
     *    a = _a;
     *    b = _b;
     *}
     */

    static int gcdex(const int& a, const int& b, int &x, int &y)
    {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }

        int tx, ty;
        int den = gcdex(b, a % b, tx, ty);

        x = ty;
        y = tx - (a / b) * ty;

        return den;
    }

    static int getReverseElement(int a, int n)
    {
        int x, y, den;
        den = gcdex(a, n, x, y);

        if (den == 1)
            return x;

        throw std::invalid_argument("gcd(a, n) != 1");
        return 0;
    }

private:
   //int a, b;
};

int main(int argc, char *argv[])
{

    //cerr << gcd::getReverseElement(7, 14);

    return 0;
}
