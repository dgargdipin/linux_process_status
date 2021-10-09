#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int hours=seconds/3600;
    string hourstring,minstring,secstring;
    if(!hours/10)hourstring="0";
    hourstring+=std::to_string(hours);
    seconds=seconds%3600;
    int mins=seconds/60;
    if(!mins/10)minstring="0";
    minstring+=std::to_string(mins);

    seconds=seconds%60;
    if (!seconds / 10) secstring = "0";
    secstring+=std::to_string(seconds);
    return hourstring+":"+minstring+":"+secstring;
    
 }