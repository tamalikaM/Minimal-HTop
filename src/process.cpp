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

// TODO: Return this process's ID
int Process::Pid() {     
  return pid_; 
 }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {     
	// add total time of children processes 
   	float total_time = LinuxParser::ActiveJiffies(Process::Pid());
    // if(Process::Pid() %10 ==0)
    //cout << " value " << total_time <<"\n";
    float seconds =  Process::UpTime();
    float cpu_usage = ((total_time /sysconf(_SC_CLK_TCK))/ seconds);
    return cpu_usage;  
}

// TODO: Return the command that generated this process
string Process::Command() {   
  return LinuxParser::Command(Process::Pid()); 
}

// TODO: Return this process's memory utilization
string Process::Ram() {  
  return LinuxParser::Ram(Process::Pid()); 
}

// TODO: Return the user (name) that generated this process
string Process::User() {   
  return LinuxParser::User(Process::Pid());
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {     
  return LinuxParser::UpTime(Process::Pid());
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process &a) {   
  return Process::CpuUtilization() > a.CpuUtilization(); 
}