#include <string>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: Day HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int h, m, s, tmp;
    h = seconds / 3600;
    tmp = seconds - h * 3600;
    m = tmp / 60;
    s = tmp % 60;
    return string(to_string(h) + ":" + to_string(m) + ":" + to_string(s));
}