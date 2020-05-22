#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::getline;
using std::cout;
using std::stol;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel>> version;
  }
  return version;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {     
  ifstream filestream(kProcDirectory + kMeminfoFilename);
    string line, key;
  	float value , memTotal, memFree;
    if (filestream.is_open()) {
      while(getline(filestream, line)) {
        istringstream linestream(line);
        while (linestream >> key >> value) {
        if (key == "MemTotal:") 
          memTotal = value;
        else if(key == "MemFree:")
          memFree = value;       
      }
    }
  }
  float memUtil = (memTotal - memFree) /memTotal;
  return memUtil; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {   
  string line;
  long up_time;
  ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    if(getline(filestream, line)) {
        istringstream linestream(line);
        if(linestream >> up_time)
          return (up_time);
    }
  }
  return 0; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {   
  return LinuxParser::UpTime()*sysconf(_SC_CLK_TCK);
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
    ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    long active_jifs = 0;
    string line, value;
     if (filestream.is_open()) {
      if(getline(filestream, line)) {
      istringstream linestream(line);
      for(int i = 1; i < 18; i++) {
        linestream >> value;
        if(i == 14 || i == 15 || i == 16 || i == 17){
          active_jifs += stol(value);
        }
      }
    }
  }
  return active_jifs;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {   
  vector<string> cpuUtil = LinuxParser::CpuUtilization();
  long sum= 0;
  for(string field:cpuUtil){
    sum+=stol(field);
  }
  return sum; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
 vector<string> cpuUtil = LinuxParser::CpuUtilization();
  long idle, iowait;
  idle = stol(cpuUtil[3]);
  iowait = stol(cpuUtil[4]);
  return idle + iowait;   
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {   
  ifstream filestream(kProcDirectory +  kStatFilename);
  vector<string> cpuUtil{};
  string line, value;
  //vector<string> cpuUtil{};
  if (filestream.is_open()) {
    if(getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> value;
      for(int i = 0; i < 10 ; i ++) {
        if(linestream >> value)
        cpuUtil.push_back(value);
      }
    }
  }
  return cpuUtil;  
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  ifstream filestream(kProcDirectory + kStatFilename);
  string key, line;
  int value = 0;
  if (filestream.is_open()) {
     while (getline(filestream, line)) {
      istringstream linestream(line);
       linestream >> key >> value; 
        if (key == "processes") {
          return value;
        }
     }
  }
  return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  ifstream filestream(kProcDirectory + kStatFilename);
  string key, line;
  int value = 0;
  if (filestream.is_open()) {
     while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> value; 
        if (key == "procs_running") {
          return value;
        }
     }
  }
  return value; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  if (filestream.is_open()) {
    if(getline(filestream, line)){
    	return line; 
    }
  }
  return "";  
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key;
  float ramKB ;
  int ramMB = 0;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
     while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> ramKB) {
        if (key == "VmSize:") {
        	ramMB = ramKB/1000;
          return to_string(ramMB);
        }
      }
     }
  }
  return "";}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key, value = "";
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
     while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
     }
  }
  return value;  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, user_name, x, uID;
  user_name = "";
  ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      while (linestream >> user_name >> x >> uID) {
        if (uID == LinuxParser::Uid(pid)) {
          return user_name;
        }
      }
    }
  }
  return user_name; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long up_time;
  string line, value;;
  ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if(getline(stream, line)) {
      istringstream linestream(line);
      for(int i = 1; i < 23; i++) {
        linestream >> value;
        
        //cout <<"value "<< value;
        if(i == 22) {
          up_time = stol(value);
      }
    }
  }
}
  return up_time/sysconf(_SC_CLK_TCK); 
}