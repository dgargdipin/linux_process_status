#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>
using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE:  once you define the function
string Format::ElapsedTime(long seconds) {
  int minutes = seconds / 60;
  int hours = minutes / 60;
  minutes = minutes % 60;
  std::ostringstream formatter;

  formatter << std::setfill('0') << std::setw(2) << std::to_string(hours) << ":"
            << std::setfill('0') << std::setw(2) << std::to_string(minutes)
            << ":" << std::setfill('0') << std::setw(2)
            << std::to_string(seconds % 60);
  return formatter.str();
}