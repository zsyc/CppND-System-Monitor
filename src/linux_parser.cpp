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

template <typename T>
T findValuebyKey(const string &keyFilter, const string &filename){
  string line, key;
  T value;
  std::ifstream filestream(LinuxParser::kProcDirectory + filename);
  if (filestream.is_open()){
    do{
      std::getline(filestream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>>key>>value;
    }
    while (key!=keyFilter);
    filestream.close();
  }
  return value;
}

template <typename T>
T getValueofFile(const string &filename){
  string line;
  T value;

  std::ifstream filestream(LinuxParser::kProcDirectory + filename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream>>value;
    filestream.close();
  }
  return value;
}

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
        if (key == filterPrettyname) {
          std::replace(value.begin(), value.end(), '_', ' ');
          filestream.close();
          return value;
        }
      }
    }
    filestream.close();
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
    stream.close();
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

// system memory utilization, Template was not used here, for avoiding too many loops
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
      if (key == filterMemTotalString) total = value;
      else if (key == filterMemFreeString) free = value;
      else if (key == filterBuffers) buf = value;
      else if (key == filterCached) cach = value;
      if (total != -1 && free != -1 && buf != -1 && cach != -1) break;
    }
    filestream.close();
    used = total - free - buf - cach;
    filestream.close();
    return used/total;
  }
  return 0.0;
}

// system uptime in Seconds
long LinuxParser::UpTime() {return getValueofFile<long>(kUptimeFilename);}

int LinuxParser::TotalProcesses() {return findValuebyKey<int>(filterProcesses,kStatFilename);}

int LinuxParser::RunningProcesses() {return findValuebyKey<int>(filterRunningProcesses,kStatFilename);}

// command associated with a process
string LinuxParser::Command(int pid) {
  string line = getValueofFile<string>(to_string(pid) + kCmdlineFilename);

  /*in "/proc/[id]/cmdline", the command was seperated with '\0' (if u read with Vim, it is '^@')
    in "ncurses_display.cpp" line 84, this "commandline" will be transfered to C String, 
    so that the string after "\0" will be ignored. To avoid this, followed*/
  std::replace(line.begin(), line.end(), '\0', ' ');

  return line;
}

// memory used by a process
string LinuxParser::Ram(int pid) {
  int size = findValuebyKey<int>(filterProcMem, to_string(pid) + kStatusFilename);
  return to_string(size/1024);
}

// user ID associated with a process
string LinuxParser::Uid(int pid) {return findValuebyKey<string>(filterUID, to_string(pid) + kStatusFilename);}

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
    stream.close();
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
    stream.close();
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
    stream.close();
  }
  return 0;
}
  