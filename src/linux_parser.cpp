#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
/*
kProcDirectory{"/proc/"};
kCmdlineFilename{"/cmdline"};
kCpuinfoFilename{"/cpuinfo"};
kStatusFilename{"/status"};
kStatFilename{"/stat"};
kUptimeFilename{"/uptime"};
kMeminfoFilename{"/meminfo"};
kVersionFilename{"/version"};
kOSPath{"/etc/os-release"};
kPasswordPath{"/etc/passwd"};
*/
using std::ifstream;
using std::istringstream;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

const string ERROR_STRING = "Error";
const int ERROR_INT = -1;
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  ifstream os_file{LinuxParser::kOSPath};
  if (!os_file) return ERROR_STRING;
  string curr_line;
  while (getline(os_file, curr_line)) {
    // std::cout<<curr_line<<std::endl;
    string candidate;
    istringstream candidate_stream{curr_line};
    bool found_pretty_name = false;
    while (getline(candidate_stream, candidate, '=')) {
      if (found_pretty_name) {
        candidate.erase(std::remove(candidate.begin(), candidate.end(), '"'),
                        candidate.end());
        return candidate;
      } else if (candidate == "PRETTY_NAME")
        found_pretty_name = true;
    }
  }
  return ERROR_STRING;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  ifstream os_file{LinuxParser::kProcDirectory + LinuxParser::kVersionFilename};
  if (!os_file) return ERROR_STRING;
  string os, os_ver, version;
  os_file >> os >> os_ver >> version;
  return version;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  const char *PATH = LinuxParser::kProcDirectory.c_str();

  DIR *dir = opendir(PATH);

  struct dirent *entry = readdir(dir);

  while (entry != NULL) {
    if (entry->d_type == DT_DIR && atoi(entry->d_name))
      pids.push_back(atoi(entry->d_name));
    entry = readdir(dir);
  }

  closedir(dir);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string mem_total, mem_free;
  ifstream mem_file{LinuxParser::kProcDirectory +
                    LinuxParser::kMeminfoFilename};
  if (!mem_file) return ERROR_INT;
  string line;
  while (getline(mem_file, line)) {
    istringstream stream{line};
    string key, value;
    stream >> key >> value;
    if (key == "MemTotal:") {
      mem_total = value;
    } else if (key == "MemFree:") {
      mem_free=value;
    }
  }
  if(mem_total.empty()||mem_free.empty())return ERROR_INT;
  float mem_total_f=stof(mem_total);
  float mem_used=mem_total_f-stof(mem_free);
  return mem_used/mem_total_f;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
