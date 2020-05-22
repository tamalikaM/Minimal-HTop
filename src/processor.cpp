#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <iostream>

using std::vector;
using std::string;
using std::stof;
using std::cout;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  	vector<string> cpuUtilValues = LinuxParser::CpuUtilization();
    float user =  stof(cpuUtilValues[LinuxParser::CPUStates::kUser_]);
    float nice = stof(cpuUtilValues[LinuxParser::CPUStates::kNice_]);
    float system = stof(cpuUtilValues[LinuxParser::CPUStates::kSystem_]);
    float idle = stof(cpuUtilValues[LinuxParser::CPUStates::kIdle_]);
    float iowait = stof(cpuUtilValues[LinuxParser::CPUStates::kIOwait_]);
    float irq = stof(cpuUtilValues[LinuxParser::CPUStates::kIRQ_]);
    float softirq = stof(cpuUtilValues[LinuxParser::CPUStates::kSoftIRQ_]);
    float steal = stof(cpuUtilValues[LinuxParser::CPUStates::kSteal_]);
    float guest = stof(cpuUtilValues[LinuxParser::CPUStates::kGuest_]);
    float guestnice = stof(cpuUtilValues[LinuxParser::CPUStates::kGuestNice_]);

    // Guest time is already accounted in usertime
    user = user - guest; // guest subtracted from user time
    nice = nice - guestnice;  // guest_nice subtracted from nice time
    // Fields existing on kernels >= 2.6
    // (and RHEL's patched kernel 2.4...)
    float idle_alltime = idle + iowait; // ioWait is added in the idleTime
    float system_alltime = system + irq + softirq;
    float virt_alltime = guest + guestnice;
    float totaltime = user + nice + system_alltime + idle_alltime + steal + virt_alltime; 
    //cout << totaltime << "\n";
    prev_idle_ = prev_idle_ + prev_iowait_;
    prev_non_idle_ = prev_idle_ + prev_nice_ + prev_system_ + prev_irq_ + prev_softirq_ + prev_steal_;
    float nonIdle = user + nice + system + irq + softirq + steal;
    prev_total_ = prev_idle_ + prev_non_idle_;
    float total = idle + nonIdle;

    // differentiate: actual value minus the previous one
    float totaldel = total - prev_total_;
    float idle__del = idle - prev_idle_;

    float CPU_Percentage = (totaldel - idle__del)/totaldel;
    
    return CPU_Percentage;
    }