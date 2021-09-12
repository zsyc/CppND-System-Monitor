#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int p):pid_(p) {}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() const{ return LinuxParser::CpuUtilization(pid_);}

string Process::Command() { return LinuxParser::Command(Pid()); }

string Process::Ram() { return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid()); }

// the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process const& b) const {return b.CpuUtilization() < this->CpuUtilization();}