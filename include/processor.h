#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
    float Utilization();

 private:
    struct cpu{
        int user, nice, system, idle, iowait, irq, softirq, steal;
    }stat1, stat2;
};

#endif