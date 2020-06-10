#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, version, kernel;
  string line;
  std::ifstream filestream(kProcDirectory + kVersionFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
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
  string line;
  string key;
  string value;
  float mem_total, mem_free;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          std::replace(value.begin(), value.end(), '_', ' ');
          mem_total = std::stoi(value);
        } else if (key == "MemFree") {
          std::replace(value.begin(), value.end(), '_', ' ');
          mem_free = std::stoi(value);
        }
      }
    }
  }
  return (mem_total - mem_free) / mem_total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  int up_time;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
  }
  return up_time;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { string line;
  string key;
  string value;
  vector<string> cpu_utilization = {};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key && key == "cpu") {
        int idx{0};
        while (linestream >> value) {
          cpu_utilization.push_back(value);
          idx ++;
        }
      }
    }
  }
  return cpu_utilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total_processes;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          total_processes = std::stoi(value);
          return total_processes;
        }
      }
    }
  }
  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int running_proccesses;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          running_proccesses = std::stoi(value);
          return running_proccesses;
        }
      }
    }
  }
  return running_proccesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string pidDirectory{"/" + std::to_string(pid)};
  string command;
  string line;
  std::ifstream filestream(kProcDirectory + pidDirectory + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string pidDirectory{"/" + std::to_string(pid)};
  string ram;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          std::replace(value.begin(), value.end(), '_', ' ');
          int ram_kb = std::stoi(value);
          ram = std::to_string(ram_kb/1000);
          return ram;
        }
      }
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string pidDirectory{"/" + std::to_string(pid)};
  string uid;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + pidDirectory + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  int uid{std::stoi(LinuxParser::Uid(pid))};
  string user;
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      int idx{0};
      while (!linestream.eof()) {
        if (idx == uid) {
          linestream >> user;
          return user;
        }
        idx ++;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string pidDirectory{"/" + std::to_string(pid)};
  long uptime{0};
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + pidDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int idx{1};
    while (idx <= 22) {
      linestream >> value;
      if (idx == 22) {
        uptime = std::stoi(value);
        return uptime / sysconf(_SC_CLK_TCK);
      }
      idx ++;
    }


  }
  return uptime;
}

// TODO: Read and return CPU utilization of a process
float LinuxParser::CpuUtilization(int pid) {
  string pidDirectory = "/" + std::to_string(pid);
  float utime, stime, cutime, cstime, starttime;
  long uptime = UpTime();
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + pidDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int idx{1};
    while (idx <= 22) {
      linestream >> value;
      if (idx == 14)
        utime = std::stoi(value);
      if (idx == 15)
        stime = std::stoi(value);
      if (idx == 16)
        cutime = std::stoi(value);
      if (idx == 17)
        cstime = std::stoi(value);
      if (idx == 22)
        starttime = std::stoi(value);
      idx ++;
    }
  }
  // First we determine the total time spent for the process:
  // We also have to decide whether we want to include the time from children processes. If we do, then we add those values to total_time:
  int total_time = utime + stime + cutime + cstime;
  // Next we get the total elapsed time in seconds since the process started:
  float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
  // Finally we calculate the CPU usage percentage:
  float cpu_usage = (total_time / sysconf(_SC_CLK_TCK)) / seconds;
  return cpu_usage;
}
