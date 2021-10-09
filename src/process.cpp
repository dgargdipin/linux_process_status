#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include<linux_parser.h>
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
float Process::CpuUtilization() {
  auto [total, idle] = LinuxParser::Accumulated_Stats();
  //   printf("%d %d %d %d\n",total,prevTotal,idle,prevIdle);

  long active = LinuxParser::ActiveJiffies(pid);
  float ans = (float)(active - prevActive) / (float)(total - prevTotal);
  //   printf("%d %d\n", active - prevActive, total - prevTotal);
  std::cout << active-prevActive << std::endl << std::endl;
  prevTotal = total;
  prevActive=active;
  process_util=ans;
  return ans;
}

// TODO: Return the command that generated this process
string Process::Command() { return command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const {
  return process_util > a.process_util;
}