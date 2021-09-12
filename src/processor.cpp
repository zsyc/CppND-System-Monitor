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
    int total, preTotal;
    int idle, preIdle;
    {
        std::ifstream filestream("/proc/stat");
        if (filestream.is_open()){
            std::getline(filestream,line);
            std::istringstream linestream(line);
            linestream>>tmp;
            linestream>>stat1.user>>stat1.nice>>stat1.system>>stat1.idle>>stat1.iowait>>stat1.irq>>stat1.softirq>>stat1.steal;
        }
    }
    sleep_for(500ms);
    {
        std::ifstream filestream("/proc/stat");
        if (filestream.is_open()){
            std::getline(filestream,line);
            std::istringstream linestream(line);
            linestream>>tmp;
            linestream>>stat2.user>>stat2.nice>>stat2.system>>stat2.idle>>stat2.iowait>>stat2.irq>>stat2.softirq>>stat2.steal;
        }
    }
    preTotal = stat1.idle + stat1.iowait + stat1.user + stat1.nice + stat1.system + stat1.irq + stat1.softirq + stat1.steal;
    total = stat2.idle + stat2.iowait + stat2.user + stat2.nice + stat2.system + stat2.irq + stat2.softirq + stat2.steal;
    preIdle = stat1.idle + stat1.iowait;
    idle = stat2.idle + stat2.iowait;
    return (float)(total-preTotal-(idle-preIdle))/(float)(total-preTotal);
}


    
        
