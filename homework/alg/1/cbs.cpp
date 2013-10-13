#include <iostream>
#include <stack>
#include <string>

using std::cout;
using std::cin;
using std::stack;
using std::string;

class BracketSequence
{
public:

    explicit BracketSequence(const string& initStr)
      : begin(initStr.begin())
      , end(initStr.end())
    {}

    bool isCorrect(int& correctPosition) const
    {
        stack<char> st;
        string::const_iterator it;

        correctPosition = 0;
        for (it = begin; it != end; ++it, ++correctPosition)
            if (isOpenBracket(*it))
                st.push(*it);
            else
                if (!st.empty() && 
                    CorrespondingBracket(st.top()) == *it
                   )
                    st.pop();
                else
                    return false;
        return st.empty();
    }

private:
    bool isOpenBracket(char b) const
    {
        switch (b) {
            case '[':
                return true;
            case '(':
                return true;
            case '{':
                return true;
        }
        return false;
    }

    char CorrespondingBracket(char b) const
    {
        switch (b) {
            case '[':
                return ']';
            case '(':
                return ')';
            case '{':
                return '}';
        }
        return ' ';
    }

    string::const_iterator begin, end;
};

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);

    int pos;
    string inputString;

    getline(cin, inputString);

    BracketSequence cbs(inputString);

    if (cbs.isCorrect(pos))
        cout << "CORRECT";
    else
        cout << pos;

    return 0;
}
