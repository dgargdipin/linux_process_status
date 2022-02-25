#include "system.h"

#include <linux_parser.h>
#include <unistd.h>

#include <cassert>
#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a containers composed of the system's processes
vector<Process>& System::Processes() {
  processes_ = {};
  vector<int> pids = LinuxParser::Pids();
  for (auto& pid : pids) {
    string user = LinuxParser::User(pid);

    string command = LinuxParser::Command(pid);

    if (command == LinuxParser::ERROR_STRING ||
        user == LinuxParser::ERROR_STRING)
      continue;

    processes_.push_back(Process(pid, user, command));
  }
  for (auto& process : processes_) {
    process.process_util = process.CpuUtilization();
  }
  sort(processes_.begin(), processes_.end());
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
long int System::UpTime() { return LinuxParser::UpTime(); }