#include <unistd.h>
#include <cstddef>
#include <string>
#include <vector>
#include <algorithm>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    processes_.clear(); // one should always refresh the vector, otherwise the vector will be more&more lager!
    for (const int &pid: LinuxParser::Pids()){
        processes_.emplace_back(pid);   //emplace_back will construct an object here. "Process process(pid)" is not needed.
    }
    std::sort(processes_.begin(),processes_.end(),[](Process&a, Process&b){return a<b;});
    return processes_;
}     

string System::Kernel() { return LinuxParser::Kernel(); }

//system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }