#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key;
  float value;
  float total=-1, free=-1, used, buf=-1, cach=-1;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal") total = value;
      else if (key == "MemFree") free = value;
      else if (key == "Buffers") buf = value;
      else if (key == "Cached") cach = value;
      if (total != -1 && free != -1 && buf != -1 && cach != -1) break;
    }
    used = total - free - buf - cach;
    return used/total;
  }
  return 0.0;
}

// system uptime in Seconds
long LinuxParser::UpTime() {
  string upt, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upt;
    return std::stol(upt);
  }
  return 0;
  }
/* According to the Answer in "https://knowledge.udacity.com/questions/199377", the followed functions doesn't need to implement. They are out of date */
// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }
/*******************************************************************************************/

int LinuxParser::TotalProcesses() {
  string line, key;
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    do{
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
    }
    while (key!="processes");
  }
  return value;
}

int LinuxParser::RunningProcesses() {
  string line, key;
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    do{
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
    }
    while (key!="procs_running");
  }
  return value;
}

// command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);// in /proc/[id]/cmdline, the command was seperated with '\0' (if u read with Vim, it is '^@')
    std::replace(line.begin(), line.end(), '\0', ' ');// in "ncurses_display.cpp" line 84, this "commandline" will be transfered to C String, so that the string after "\0" will be ignored
    return line;
  }
  return string();
}

// memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key;
  int size;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    do {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' '); 
      std::istringstream linestream(line);
      linestream>>key>>size;
    }
    while (key!="VmSize");
  }
  return to_string(size/1024);
}

// user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, id;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    do {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>>key>>id;
    }
    while (key!="Uid");
    return id;
  }
  return string();    // if stream was not correctly opend.
}

// the user associated with a process
string LinuxParser::User(int pid){
  string line, name, id, tmp, userId = Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    do {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>>name>>tmp>>id;
    }
    while(id != userId);
  }
  return name;
}

// uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, cloktik;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), '(', ' ');
    std::replace(line.begin(), line.end(), ')', ' ');
    std::istringstream linestream(line);
    for (int i=0; i<22; ++i) linestream>>cloktik;
    return LinuxParser::UpTime() - std::stol(cloktik)/sysconf(_SC_CLK_TCK);
  }
  return 0;
}

float LinuxParser::CpuUtilization(int pid) {
  long uptime = UpTime();
  string line, tmp;
  long int utime, stime, cutime, cstime, starttime, total_time, seconds;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), '(', ' ');
    std::replace(line.begin(), line.end(), ')', ' ');
    std::istringstream linestream(line);
    for (int i=0;i<13;++i) linestream>>tmp;
    linestream>>utime>>stime>>cutime>>cstime;
    for (int i=0;i<5;++i) linestream>>starttime;
    total_time = utime + stime + cutime + cstime;
    seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
    return ((float)total_time/sysconf(_SC_CLK_TCK)/seconds);
  }
  return 0;
}
  