#include <string>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int h, m, s, tmp;
    h = seconds / 3600;
    tmp = seconds - h * 3600;
    m = tmp / 60;
    s = tmp % 60;
    
    /* force to use 2-digit time, otherwise because of the length's changing, the time will be showed wrong
    https://knowledge.udacity.com/questions/155686 
    some other interesting approach using Lib<chrono>: https://www.youtube.com/watch?v=P32hvk8b13M */
    string st_s, min_s,sek_s;
    st_s = h<10? "0"+to_string(h) : to_string(h);
    min_s = m<10? "0"+to_string(m) : to_string(m);
    sek_s = s<10? "0"+to_string(s) : to_string(s);
    
    return string(st_s + ":" + min_s + ":" + sek_s);
}