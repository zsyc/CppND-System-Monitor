#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
    float Utilization();

 private:
    struct cpu{
        int user, nice, system, idle, iowait, irq, softirq, steal;
    }stat;
    int _preIdle=-1, _preTotal=-1;
};

#endif