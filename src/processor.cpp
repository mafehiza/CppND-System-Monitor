#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int user, nice, system, idle, iowait, irq, softirq, steal;
    float Idle, NonIdle, Total;
    float CPU_Percentage;
    vector<string> cpu_utilization = LinuxParser::CpuUtilization();
    user = std::stoi(cpu_utilization[0]);
    nice = std::stoi(cpu_utilization[1]);
    system = std::stoi(cpu_utilization[2]);
    idle = std::stoi(cpu_utilization[3]);
    iowait = std::stoi(cpu_utilization[4]);
    irq = std::stoi(cpu_utilization[5]);
    softirq = std::stoi(cpu_utilization[6]);
    steal = std::stoi(cpu_utilization[7]);
    Idle = idle + iowait;
    NonIdle = user + nice + system + irq + softirq + steal;
    Total = Idle + NonIdle;
    // differentiate: actual value minus the previous one
    CPU_Percentage = (Total - Idle)/Total;
    return CPU_Percentage;
}