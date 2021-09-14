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

// Key strings
const std::string filterPrettyname("PRETTY_NAME");
const std::string filterProcesses("processes");
const std::string filterRunningProcesses("procs_running");
  //memory
const std::string filterMemTotalString("MemTotal");
const std::string filterMemFreeString("MemFree");
const std::string filterBuffers("Buffers");
const std::string filterCached("Cached");

const std::string filterCpu("cpu");
const std::string filterUID("Uid");
  // according last review: "VmRSS" instead of "VmSize", because we need phsical RAM instead of Virtual RAM
const std::string filterProcMem("VmRSS"); 



// System
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

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
float CpuUtilization(int pid);
};  // namespace LinuxParser

#endif