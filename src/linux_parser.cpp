#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <array>
#include <iomanip>
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
using std::cout;
using std::endl;


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
      mem_free = value;
    }
  }
  if (mem_total.empty() || mem_free.empty()) return ERROR_INT;
  float mem_total_f = stof(mem_total);
  float mem_used = mem_total_f - stof(mem_free);
  return mem_used / mem_total_f;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  ifstream uptime_file{LinuxParser::kProcDirectory +
                       LinuxParser::kUptimeFilename};
  if (!uptime_file) return ERROR_INT;
  long uptime;
  uptime_file >> uptime;
  // printf("%ld\n",uptime);
  return uptime;//inseconds
}

std::array<long, LinuxParser::PROC_STAT_SIZE>
LinuxParser::parse_proc_stat_cpu() {
  ifstream stat_stream{LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename};
  if (!stat_stream) return {};
  string curr_line;
  while (getline(stat_stream, curr_line)) {
    istringstream line_stream{curr_line};
    string cpu;
    line_stream >> cpu;
    if (cpu != "cpu") continue;
    long user, nice, system, idle, iowait, irq, softirq, steal, guest,
        guest_nice;
    line_stream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
        steal >> guest >> guest_nice;
    return {user, nice,    system, idle,  iowait,
            irq,  softirq, steal,  guest, guest_nice};
  }
  return {};
}

std::array<long, LinuxParser::ACCUM_STAT_SIZE>
LinuxParser::Accumulated_Stats() {
  
  auto [user, nice, system, idle, iowait, irq, softIrq, steal, guest,
        guest_nice] = parse_proc_stat_cpu();
  
  
  user = user - guest;
  nice = nice - guest_nice;
  long idlealltime = idle + iowait;
  long systemalltime = system + irq + softIrq;
  long virtalltime = guest + guest_nice;
  long totaltime =
      user + nice + systemalltime + idlealltime + steal + virtalltime;
  return {totaltime, idlealltime};
}
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  auto [totaltime, idle] = Accumulated_Stats();
  return totaltime;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE:  once you define the function
long LinuxParser::ActiveJiffies(int pid ) {
  ifstream pid_stream{LinuxParser::kProcDirectory + to_string(pid) +
                      LinuxParser::kStatFilename};
  if (!pid_stream) return ERROR_INT;
  string pid_info;
  getline(pid_stream, pid_info);
  istringstream pid_info_stream{pid_info};
  // cout<<pid_info<<endl;
  string comm;
  char state;
  int ppid,pgrp,session,tty_nr,tpgid;
  unsigned int flags;
  unsigned long minflt,cminflt,majflt,cmajflt,utime,stime;
  long int cutime,cstime;
  pid_info_stream>>pid>>comm>>state>>ppid>>pgrp>>session>>tty_nr>>tpgid>>flags>>minflt>>cminflt>>majflt>>cmajflt>>utime>>stime>>cutime>>cstime;
  long jiffies = utime + stime;
  // printf("%d:%ld \n",pid,jiffies);
   return jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return Jiffies() - IdleJiffies(); }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto [totaltime, idle] = Accumulated_Stats();
  return idle;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
std::array<int, LinuxParser::PROCESS_INFO_SIZE> LinuxParser::process_info() {
  ifstream stat_stream{LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename};
  if (!stat_stream) return {};
  string curr_line;
  int total_procs, running_procs;
  while (getline(stat_stream, curr_line)) {
    istringstream line_stream{curr_line};
    string processes;
    line_stream >> processes;
    if (processes != "processes" && processes != "procs_running") continue;
    if (processes == "processes") {
      line_stream >> total_procs;
    } else
      line_stream >> running_procs;
  }
  return {running_procs, total_procs};
};
int LinuxParser::TotalProcesses() {
  auto [running_procs, total_procs] = LinuxParser::process_info();
  return total_procs;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  auto [running_procs, total_procs] = LinuxParser::process_info();
  return running_procs;
}

// TODO: Read and return the command associated with a process
// REMOVE:  once you define the function
string LinuxParser::Command(int pid ) {
  ifstream command_stream{LinuxParser::kProcDirectory + to_string(pid) +
                          LinuxParser::kCmdlineFilename};
  if (!command_stream) return ERROR_STRING;
  string command;
  getline(command_stream, command);
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE:  once you define the function
string LinuxParser::Ram(int pid ) {
  ifstream status_stream{LinuxParser::kProcDirectory + to_string(pid) +
                         LinuxParser::kStatusFilename};
  string curr_line_string;
  while (getline(status_stream, curr_line_string)) {
    istringstream curr_line_stream{curr_line_string};
    string key, size, unit;
    curr_line_stream >> key;

    if (key == "VmSize:") {
      curr_line_stream >> size >> unit;
      double sizeInMb = std::stod(size) / 1024;
      std::ostringstream ram_precision_stream;
      ram_precision_stream << std::fixed << std::setprecision(1) << sizeInMb;
      string ram = ram_precision_stream.str()+" ";
      // cout<<ram<<endl;
      return ram;
    }
  }
  return ERROR_STRING;
}

// TODO: Read and return the user ID associated with a process
// REMOVE:  once you define the function
string LinuxParser::Uid(int pid ) {
  ifstream status_stream{LinuxParser::kProcDirectory + to_string(pid) +
                         LinuxParser::kStatusFilename};
  string curr_line_string;
  while (getline(status_stream, curr_line_string)) {
    istringstream curr_line_stream{curr_line_string};
    string key;
    curr_line_stream >> key;
    if (key == "Uid:") {
      string uid;
      curr_line_stream >> uid;
      return uid;
    }
  }
  return ERROR_STRING;
}

// TODO: Read and return the user associated with a process
// REMOVE:  once you define the function
string LinuxParser::User(int pid ) {
  ifstream status_stream{LinuxParser::kPasswordPath};
  string curr_line_string;
  string uid_to_catch = LinuxParser::Uid(pid);
  while (getline(status_stream, curr_line_string)) {
    std::replace(curr_line_string.begin(), curr_line_string.end(), ':', ' ');
    istringstream curr_line_stream{curr_line_string};
    string name, password, uid;
    curr_line_stream >> name >> password >> uid;
    if (uid == uid_to_catch) {
      return name;
    }
  }
  return ERROR_STRING;
}

// TODO: Read and return the uptime of a process
// REMOVE:  once you define the function
long LinuxParser::StartTime(int pid ) {
  ifstream pid_stream{LinuxParser::kProcDirectory + to_string(pid) +
                      LinuxParser::kStatFilename};
  if (!pid_stream) return ERROR_INT;
  string pid_info;
  getline(pid_stream, pid_info);
  istringstream pid_info_stream{pid_info};
  // cout<<pid_info<<endl;
  string comm;
  char state;
  int ppid, pgrp, session, tty_nr, tpgid;
  unsigned int flags;
  unsigned long minflt, cminflt, majflt, cmajflt, utime, stime;
  long int cutime, cstime,priority,nice,threads,itrealvalue,starttime;
  pid_info_stream >> pid >> comm >> state >> ppid >> pgrp >> session >>
      tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >>
      utime >> stime >> cutime >> cstime>>priority>>nice>>threads>>itrealvalue>>starttime;
  return starttime;//measured in clock ticks
}
