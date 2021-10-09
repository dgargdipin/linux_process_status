#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <cassert>
#include<iostream>
#include "process.h"
#include "processor.h"
#include "system.h"
#include<linux_parser.h>
using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a containers composed of the system's processes
vector<Process>& System::Processes() {
  processes_={};
  vector<int> pids=LinuxParser::Pids();
  for(auto &pid:pids){

    string user=LinuxParser::User(pid);
    if(user==LinuxParser::ERROR_STRING)continue;
    string command=LinuxParser::Command(pid);
    Process newProcess=Process(pid,user,command);
    processes_.push_back(newProcess);
  }
  sort(processes_.begin(),processes_.end());
  // for(int i=0;i<processes_.size()-1;i++){
  //   assert(processes_[i].process_util>processes_[i+1].process_util);
  // }
      // for(auto &process:processes_){
      //   printf("%f ", process.CpuUtilization());
      // }
      return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime();
}