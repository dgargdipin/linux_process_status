#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, std::string user, std::string command);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization() const;            // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long StartTime() const;                       // TODO: See src/process.cpp
  long UpTime() const;                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  float process_util=0;

  // TODO: Declare any necessary private members
 private:
  int pid;
  std::string user, command;
  long prevActive=0,prevTotal=0;
};

#endif