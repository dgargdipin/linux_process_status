#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};
const std::string ERROR_STRING = "Error";
const int ERROR_INT = -1;
const int PROC_STAT_SIZE = 10;
const int ACCUM_STAT_SIZE = 2;
const int PROCESS_INFO_SIZE = 2;
const int uTime_idx = 14;
const int startTime_idx = 22;
const int RAM_WIDTH = 7;
const int COMMAND_WIDTH = 150;
const bool INCLUDE_CU_CS_TIME = false;
// bool COUNT_CHILD_PROC_TIME=false;
// System
std::array<long, PROC_STAT_SIZE> parse_proc_stat_cpu();
std::array<long, ACCUM_STAT_SIZE> Accumulated_Stats();
std::array<int, PROCESS_INFO_SIZE> process_info();

float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
void padWithChar(std::string& command, char toFillWith, int n);
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
long int StartTime(int pid);
};  // namespace LinuxParser

#endif