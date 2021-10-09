#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include<linux_parser.h>
#include<cassert>
using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid,string user,string command){
    Process::pid=pid;
    Process::user=user;
    Process::command=command;
}
// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  long total_time=LinuxParser::ActiveJiffies(pid);
  long seconds=LinuxParser::UpTime()-Process::UpTime();
  float ans=((float)total_time/sysconf(_SC_CLK_TCK))/(float)seconds;
  return ans;

  
}

// TODO: Return the command that generated this process
string Process::Command() { return command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const {
  return LinuxParser::UpTime(pid) / sysconf(_SC_CLK_TCK);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const {
  return CpuUtilization() > a.CpuUtilization();
}