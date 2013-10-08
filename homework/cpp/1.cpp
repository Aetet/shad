#include <ctime> 
#include <iostream>
#include <string>
#include <stdexcept>

using std::string;
using std::cerr;
using std::cout;

class DateManipulator
{
public:
    DateManipulator(const string& date)
    {
        if (strptime(date.c_str(), timeFormat.c_str(), &time) == NULL)
            throw std::invalid_argument("bad date format");
    }

    string getDayOfWeek() const
    {
        return dayOfWeek[time.tm_wday];
    }

    bool isLeapYear() const
    {
        int year = 1900 + time.tm_year;
        if (year % 400 == 0)
            return true;

        if (year % 4 == 0 && year % 100 != 0)
            return true;

        return false;
    }

private:
    struct tm time;
    static const string timeFormat;
    static const string dayOfWeek[];
};

const string DateManipulator::timeFormat = "%Y-%m-%d";
const string DateManipulator::dayOfWeek[] = {"Sunday", "Monday", "Tuesday",
    "Wednesday", "Thursday", "Friday", "Saturday"};

int main(int argc, char *argv[])
{

    DateManipulator dm("1600-10-08");

    cerr << dm.isLeapYear() << std::endl;
    cerr << dm.getDayOfWeek();

    return 0;
}
