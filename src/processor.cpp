#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <iostream>

using std::vector;
using std::string;
using std::stof;
using std::cout;

// Return the aggregate CPU utilization
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
    idle = idle + iowait;
    float nonIdle = user + nice + system + irq + softirq + steal;
    float total = idle + nonIdle;
    
    float totald = total - prev_total_;
    float idled = idle - prev_idle_;
    
    prev_total_ = total;
    prev_idle_ = idle;

    return (totald - idled)/totald;
}