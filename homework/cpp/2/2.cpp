#include <iostream>
#include <vector>
#include <iterator>

using std::vector;
using std::cin;
using std::cout;
using std::cerr;
using std::iterator;
using std::ostream;
using std::endl;

template<typename T>
class Polynomial
{
public:
    Polynomial()
    {}

    Polynomial(T coefficient)
        : coefficients(1, coefficient)
    {}

    template<typename InputIterator>
    Polynomial(InputIterator begin
             , InputIterator end)
        : coefficients(begin, end)
    {}

    T& operator[](size_t i)
    {
        if (i >= coefficients.size())
            coefficients.resize(i + 1);

        return coefficients[i];
    }

    const T& operator[](size_t i) const
    {
        return coefficients[i];
    }

    bool operator==(const Polynomial& polynom) const
    {
        if (polynom.degree() == this->degree()) {
            for (int i = 0; i <= polynom.degree(); ++i)
            // float comparsion problem, mb better abs(delt) > eps?
                if (polynom.coefficients[i] != this->coefficients[i])
                    return false;
        }
        else
            return false;

        return true;
    }

    bool operator!=(const Polynomial& polynom) const
    {
        return !(*this == polynom);
    }

    Polynomial operator%(const Polynomial& polynom) const
    {
        return division(polynom, 1);
    }

    Polynomial operator/(const Polynomial& polynom) const
    {
        return division(polynom, 0);
    }

    const Polynomial& operator+=(const Polynomial& polynom)
    {
        for (int i = 0; i <= polynom.degree(); ++i)
            (*this)[i] += polynom[i];

        updateDegree();
        return *this;
    }

    Polynomial operator+(const Polynomial& polynom) const
    {
        Polynomial newPolynom(*this);
        return newPolynom += polynom;
    }

    const Polynomial& operator*=(T multiplier)
    {
        for (int i = 0; i <= this->degree(); ++i) {
            (*this)[i] *= multiplier;
        }

        return *this;
    }

    Polynomial operator*(T multiplier) const
    {
        Polynomial newPolynom(*this);
        return  newPolynom *= multiplier;
    }

    const Polynomial& operator-=(const Polynomial& polynom)
    {
        for (int i = 0; i <= polynom.degree(); ++i)
            (*this)[i] -= polynom[i];

        updateDegree();
        return *this;
    }

    Polynomial operator-(const Polynomial& polynom) const
    {
        Polynomial newPolynom(*this);
        return newPolynom -= polynom;
    }

    Polynomial operator*(const Polynomial& polynom) const
    {
        // fft can do it faster
        Polynomial<T> newPolynom;
        for (int i = 0; i <= this->degree(); ++i) {
            for (int j = 0; j <= polynom.degree(); ++j) {
                newPolynom[i + j] += (*this)[i] * polynom[j];
            }
        }

        return newPolynom;
    }

    const Polynomial& operator*=(const Polynomial& polynom)
    {
        *this = *this * polynom;
        return *this;
    }

    bool operator<(const Polynomial& polynom) const
    {
        if (this->degree() == polynom.degree()) {
            for (int i = this->degree(); i >= 0; --i) {
            // floating comparsion
                if ((*this)[i] < polynom[i])
                    return true;
            }
        }

        return (this->degree() < polynom.degree());
    }

    bool operator<=(const Polynomial& polynom) const
    {
        return (*this == polynom) || (*this < polynom);
    }

    const T operator()(T point) const
    {
        T res = 0;
        T value = 1;

        for (int i = 0; i <= this->degree(); ++i) {
            res += value * (*this)[i];
            value *= point;
        }

        return res;
    }


    int degree()
    {
        updateDegree();
        return this->coefficients.size() - 1;
    }

    int degree() const
    {
        return this->coefficients.size() - 1;
    }

    typename vector<T>::iterator begin()
    {
        return coefficients.begin();
    }

    typename vector<T>::iterator end()
    {
        return coefficients.end();
    }

    typename vector<T>::const_iterator begin() const
    {
        return coefficients.cbegin();
    }

    typename vector<T>::const_iterator end() const
    {
        return coefficients.cend();
    }

    friend ostream& operator<<(ostream& stream, const Polynomial &polynom)
    {
        // from 0 to degree()
        for (auto it  = polynom.begin();
                  it != polynom.end(); ++it)
            cout << *it << " ";
        return stream;
    }

private:

    void updateDegree()
    {
        while (!this->coefficients.empty() && this->coefficients.back() == 0)
            this->coefficients.pop_back();
    }

    Polynomial division(const Polynomial& divisor, bool getModule) const
    {
        Polynomial<T> module(*this), result;


        while (divisor <= module) {
            Polynomial mult;
            mult[module.degree() - divisor.degree()]
                // coefficients should be good
                = module.coefficients.back() / divisor.coefficients.back();

            //cerr << "cerr: " << endl;
            //cerr << module << endl;
            //cerr << divisor << endl;
            //cerr << mult << endl;

            module -= divisor * mult;
            if (!getModule)
                result += mult;
        }

        if (getModule)
            return module;
        else
            return result;
    }

    vector<T> coefficients;
};

template<typename T>
Polynomial<T> operator,(Polynomial<T> a, Polynomial<T> b)
{
    if (a.degree() == -1)
        return b;
    return (b % a, a);
}

int main(int argc, char *argv[])
{
    vector<int> coefs = {1, 53, 11};
    //Polynomial<int> p(coefs.begin(), coefs.end());
    //Polynomial<int> p2(7);

    //p += p2;
    //p[0] = 3;
    //cout << (p == p2) << endl;
    //cout << p + p << endl;
    //p *= p;

    Polynomial<int> p;
    Polynomial<int> p2;

    p[0] = 1;
    p[1] = 3;
    p[2] = 1;
    p2[0] = 2;
    p2[1] = 1;

    p *= p2;
    ++p[0];
    ++p[1];
    ++p[1];
    cout << (p % p2) << endl;

    Polynomial<int> pp, pp2;
    pp[0] = 1;
    pp[1] = 2;
    pp[2] = 3;
    pp[3] = 4;
    pp2[0] = 1;
    pp2[1] = 2;
    pp2[2] = 3;
    pp2[3] = 4;
    cout << (pp, pp2);
    return 0;
}
