#ifndef PROCESSESINFO_H
#define PROCESSESINFO_H

#include <unordered_map>
#include <string>
#include <regex>
#include <memory>

class ProcessesInfo {

    static const std::regex pattern;

    struct process {

        double cpu_usage;
        double memory_usage;
        std::string time;
    };

    using ProcessT = struct process;

    static std::unordered_map<unsigned, std::unique_ptr<ProcessT>> processesInfo;

    static void clear();
    static void parseProcessInfo(const std::string& process_info);

public:
    ProcessesInfo() = delete;
    ProcessesInfo(const ProcessesInfo&) = delete;
    ProcessesInfo& operator=(const ProcessesInfo&) = delete;
    ProcessesInfo(ProcessesInfo&&) = delete;
    ProcessesInfo& operator=(ProcessesInfo&&) = delete;

    static void update();
    static double getCPUUsageByPID(unsigned pid);
    static double getMemoryUsageByPID(unsigned pid);
    static const std::string& getTimeByPID(unsigned pid);
};

#endif // PROCESSESINFO_H
