#include <iostream>
#include <vector>
#include <string>
#include <complex>
#include <cmath>
#include <algorithm>

// using namespace std;
using std::cin;
using std::cout;
using std::vector;
using std::complex;
using std::string;
using std::reverse;
using std::max;
using std::endl;
using std::cerr;

typedef complex<double> base;

class TSolver
{
public:
    TSolver(const string& text, const string& substr)
        : text_(text), substr_(substr)
    {}

    vector<int> getPolynom(char reduceChar, const string& str)
    {
        vector<int> ans;
        for (int i = 0; i < str.size(); ++i) {
            ans.push_back(str[i] == reduceChar);
        }

        return ans;
    }

    void fft (vector<base> & vec, bool invert) {
        int sz = static_cast<int>(vec.size());
        if (sz == 1)
            return;

        vector<base> ft (sz / 2),  sd (sz / 2);
        for (int i = 0, j = 0; i < sz; i += 2, ++j) {
            ft[j] = vec[i];
            sd[j] = vec[i + 1];
        }
        fft (ft, invert);
        fft (sd, invert);

        double ang = 2 * M_PI / sz * (invert ? -1 : 1);
        base root(1),  rootn (cos(ang), sin(ang));
        for (int i = 0; i < sz / 2; ++i) {
            vec[i] = ft[i] + root * sd[i];
            vec[i + sz / 2] = ft[i] - root * sd[i];
            if (invert)
                vec[i] /= 2,  vec[i + sz / 2] /= 2;
            root *= rootn;
        }
    }

    void multiply (const vector<int> & ft,
                   const vector<int> & sd,
                   vector<int>& res) {
        vector<base> fa (ft.begin(), ft.end()),
                     fb (sd.begin(), sd.end());
        size_t deg = 1;
        while (deg < max (ft.size(), sd.size()))
            deg <<= 1;
        deg <<= 1;
        fa.resize (deg),  fb.resize (deg);

        fft (fa, false),  fft (fb, false);
        for (size_t i = 0; i < deg; ++i) {
            fa[i] *= fb[i];
        }

        fft (fa, true);

        res.resize (deg);
        for (size_t i = 0; i < deg; ++i) {
            res[i] += int (fa[i].real() + 0.5);
        }
    }

    int getAns()
    {
        vector <char> rcs = {'A', 'G', 'T', 'C'};
        vector <int> ans;

        for (int i = 0; i < rcs.size(); ++i) {
            vector <int> ttext(getPolynom(rcs[i], text_));
            vector <int> tsubstr(getPolynom(rcs[i], substr_));
            reverse(ttext.begin(), ttext.end());
            multiply(ttext, tsubstr, ans);
        }

        int ml = substr_.size() - 1;
        int tret = ml;

        for (int i = ml; i < text_.size(); ++i) {
            if (ans[i] >= ans[tret]) {
                tret = i;
            }
        }

/*
 *        for (int i = 0; i < ans.size(); ++i) {
 *            cerr << "ans[" << i << "] = " << ans[i] << endl;
 *        }
 */
        return static_cast<int>(text_.size() - tret);
    }

/*
 *    void test()
 *    {
 *        vector<int> ttext(  getPolynom('e', text_));
 *        vector<int> tsubstr(getPolynom('e', substr_));
 *
 *        reverse(ttext.begin(), ttext.end());
 *        voc
 *        for (int i = 0; i < ans.size(); ++i) {
 *            cerr << ans[i] << endl;
 *        }
 *    }
 */

private:
    string text_, substr_;
};

int main(int argc, char *argv[])
{
    string text, substr;
    cin >> text >> substr;
    TSolver ts(text, substr);
    cout << ts.getAns() << endl;
    return 0;
}
