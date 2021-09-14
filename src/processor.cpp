#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include "processor.h"

using std::string;
using std::this_thread::sleep_for;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.

//the aggregate CPU utilization
float Processor::Utilization() {
    string line, tmp;
    int idle, total;

    std::ifstream filestream("/proc/stat");
    if (filestream.is_open()){
        std::getline(filestream,line);
        std::istringstream linestream(line);
        linestream>>tmp;
        linestream>>stat.user>>stat.nice>>stat.system>>stat.idle>>stat.iowait>>stat.irq>>stat.softirq>>stat.steal;
        filestream.close();
    }
    total = stat.idle + stat.iowait + stat.user + stat.nice + stat.system + stat.irq + stat.softirq + stat.steal;
    idle = stat.idle + stat.iowait;
    if (_preIdle!=-1 && _preTotal!=-1) return static_cast<float>(total-_preTotal-(idle-_preIdle))/static_cast<float>(total-_preTotal);
    else{
        _preIdle = idle;
        _preTotal = total;
        sleep_for(1000ms);
        std::ifstream filestream("/proc/stat");
        if (filestream.is_open()){
            std::getline(filestream,line);
            std::istringstream linestream(line);
            linestream>>tmp;
            linestream>>stat.user>>stat.nice>>stat.system>>stat.idle>>stat.iowait>>stat.irq>>stat.softirq>>stat.steal;
            filestream.close();
        }
        total = stat.idle + stat.iowait + stat.user + stat.nice + stat.system + stat.irq + stat.softirq + stat.steal;
        idle = stat.idle + stat.iowait;
        return static_cast<float>(total-_preTotal-(idle-_preIdle))/static_cast<float>(total-_preTotal);
    } 
}    
        
