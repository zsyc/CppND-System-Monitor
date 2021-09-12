#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    processes_.clear(); // one should always refresh the vector, otherwise the vector will be more&more lager!
    for (int pid: LinuxParser::Pids()){
        Process process(pid);
        processes_.emplace_back(process);
    }
    std::sort(processes_.begin(),processes_.end(),[](Process&a, Process&b){return a<b;});
    return processes_;
}     

std::string System::Kernel() { return LinuxParser::Kernel(); }

//system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }