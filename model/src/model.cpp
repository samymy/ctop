#include "model.h"
#include "processesinfo.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <regex>
#include <sys/types.h>
#include <dirent.h>

void Model::setCurrentTime()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(now);
    currentTime = std::string(std::ctime(&time)).substr(11, 8);
}

void Model::setRunningTime()
{
    std::ifstream file("/proc/uptime");
    if (file) {

        std::string uptime_str;
        file >> uptime_str;

        double uptime = std::stod(uptime_str);
        std::stringstream ss;
        ss << static_cast<unsigned>(uptime / 60 / 60) << ":" << static_cast<unsigned>(uptime / 60) % 60;
        runningTime = ss.str();
    }
}

void Model::setUserSessions()
{
    std::ifstream file("/proc/key-users");
    if (file) {

        std::string line;
        unsigned cnt = 0;
        while (std::getline(file, line))
            ++cnt;
        userSessions = cnt;
    }
}

void Model::setLoadAverages()
{
    std::ifstream file("/proc/loadavg");
    if (file) {

        std::string load_average;

        file >> load_average;
        load_average[1] = ',';
        loadAverageOverOneMinute = std::stod(load_average);
        file >> load_average;
        load_average[1] = ',';
        loadAverageOverFiveMinutes = std::stod(load_average);
        file >> load_average;
        load_average[1] = ',';
        loadAverageOverFifteenMinutes = std::stod(load_average);
    }
}

void Model::setTaskCounts()
{
    runningTasksCount = std::count_if(processes.begin(), processes.end(), [](const ProcessT* process){
        return process->status == 'R';
    });
    sleepingTasksCount = std::count_if(processes.begin(), processes.end(), [](const ProcessT* process){
        return process->status == 'S';
    });
    stoppedTasksCount = std::count_if(processes.begin(), processes.end(), [](const ProcessT* process){
        return process->status == 'T';
    });
    zombieTasksCount = std::count_if(processes.begin(), processes.end(), [](const ProcessT* process){
        return process->status == 'Z';
    });
}

void Model::setCPUUsageTime()
{
    std::ifstream file("/proc/stat");
    if (file) {

        file.seekg(5);
        std::string cpu_usage_time;

        file >> cpu_usage_time;
        userspaceCPUUsageTime = std::stod(cpu_usage_time) / CLOCKS_PER_SEC * 10;
        file >> cpu_usage_time;
        NIProcessesCPUUsageTime = std::stod(cpu_usage_time) / CLOCKS_PER_SEC * 10;
        file >> cpu_usage_time;
        kernelspaceCPUUsageTime = std::stod(cpu_usage_time) / CLOCKS_PER_SEC * 10;
        file >> cpu_usage_time;
        file >> cpu_usage_time;
        IOCPUUsageTime = std::stod(cpu_usage_time) / CLOCKS_PER_SEC * 10;
        file >> cpu_usage_time;
        hardwareInterruptsCPUUsageTime = std::stod(cpu_usage_time) / CLOCKS_PER_SEC * 10;
        file >> cpu_usage_time;
        softwareInterruptsCPUUsageTime = std::stod(cpu_usage_time) / CLOCKS_PER_SEC * 10;
        file >> cpu_usage_time;
        stealCPUUsageTime = std::stod(cpu_usage_time) / CLOCKS_PER_SEC * 10;

        idleCPUTime = 100 - userspaceCPUUsageTime -
                            NIProcessesCPUUsageTime -
                            kernelspaceCPUUsageTime -
                            IOCPUUsageTime -
                            hardwareInterruptsCPUUsageTime -
                            softwareInterruptsCPUUsageTime -
                            stealCPUUsageTime;
    }
}

void Model::setMemorySizes()
{
    std::ifstream file("/proc/meminfo");
    if (file) {

        file.seekg(9);
        std::string size;
        std::getline(file, size);

        unsigned total = std::stol(size);

        file.seekg(8, std::ios_base::cur);
        std::getline(file, size);
        freeMemorySize = std::stol(size);
        file.seekg(13, std::ios_base::cur);
        std::getline(file, size);
        availableMemorySize = std::stol(size);
        file.seekg(8, std::ios_base::cur);
        std::getline(file, size);
        cacheMemorySize = std::stol(size);
        file.seekg(7, std::ios_base::cur);
        std::getline(file, size);
        cacheMemorySize += std::stol(size);
        usedMemorySize = total - freeMemorySize - cacheMemorySize;
        file.seekg(9 * 28 + 10, std::ios_base::cur);
        std::getline(file, size);
        total = std::stol(size);
        file.seekg(9, std::ios_base::cur);
        std::getline(file, size);
        freeSwapSize = std::stol(size);
        usedSwapSize = total - freeSwapSize;
    }
}

const ProcessT* Model::getProcessByPID(unsigned pid)
{
    ProcessT* process = new ProcessT;

    process->pid = pid;
    process->cpu_usage = ProcessesInfo::getCPUUsageByPID(pid);
    process->memory_usage = ProcessesInfo::getMemoryUsageByPID(pid);
    process->time = ProcessesInfo::getTimeByPID(pid);

    std::stringstream ss;
    ss << "/proc/" << pid << "/stat";
    std::ifstream stat(ss.str());
    if (stat) {

        std::string token;
        unsigned cnt = 0;
        while (stat >> token) {
            ++cnt;
            switch (cnt) {

            case 2:
                process->user = token.substr(1, token.size() - 2);
                break;
            case 3:
                process->status = token[0];
                break;
            case 18:
                process->priority = std::stoi(token);
                break;
            case 19:
                process->nice_value = std::stoi(token);
                break;
            default:
                break;
            }
        }
    }

    ss.str("");
    ss << "/proc/" << pid << "/status";
    std::ifstream status(ss.str());
    if (status) {

        std::string line;
        const std::regex vm_size { R"(VmSize:\s+(\d+)\s+kB)" };
        const std::regex vm_rss { R"(VmRSS:\s+(\d+)\s+kB)" };
        const std::regex rss_file { R"(RssFile:\s+(\d+)\s+kB)" };

        while (std::getline(status, line)) {

            if (std::regex_match(line, vm_size))
                process->virtual_memory = std::stol(std::regex_replace(line, vm_size, "$1"));
            if (std::regex_match(line, vm_rss))
                process->resident_memory = std::stol(std::regex_replace(line, vm_rss, "$1"));
            if (std::regex_match(line, rss_file))
                process->shared_memory = std::stol(std::regex_replace(line, rss_file, "$1"));
        }
    }

    ss.str("");
    ss << "/proc/" << pid << "/cmdline";
    std::ifstream cmdline(ss.str());
    if (cmdline) {

        std::string line;
        std::getline(cmdline, line);
        process->command = line;
    }

    if (process->status == 'C') {

        delete process;
        return nullptr;
    }

    return process;
}

void Model::setProcesses()
{
    clear();

    DIR* dir = opendir("/proc");
    if (dir) {

        struct dirent* entry;
        unsigned pid;
        const ProcessT* process;

        ProcessesInfo::update();
        while ((entry = readdir(dir))) {

            pid = atoi(entry->d_name);
            if (pid)
               if ((process = getProcessByPID(pid)))
                    processes.push_back(process);
        }
    }
    std::sort(processes.begin(), processes.end(), [](const ProcessT* a, const ProcessT* b){
        return a->cpu_usage > b->cpu_usage;
    });
}

void Model::clear()
{
    for (const auto& process : processes)
        delete process;
    processes.clear();
}

void Model::readProcInfo()
{
    setCurrentTime();
    setRunningTime();
    setUserSessions();
    setLoadAverages();
    setCPUUsageTime();
    setMemorySizes();
    setProcesses();
    setTaskCounts();
}

const std::string& Model::getCurrentTime() const
{
    return currentTime;
}

const std::string& Model::getRunningTime() const
{
    return runningTime;
}

unsigned Model::getUserSessions() const
{
    return userSessions;
}

double Model::getLoadAverageOverOneMinute() const
{
    return loadAverageOverOneMinute;
}

double Model::getLoadAverageOverFiveMinutes() const
{
    return loadAverageOverFiveMinutes;
}

double Model::getLoadAverageOverFifteenMinutes() const
{
    return loadAverageOverFifteenMinutes;
}

unsigned Model::getRunningTasksCount() const
{
    return runningTasksCount;
}

unsigned Model::getSleepingTasksCount() const
{
    return sleepingTasksCount;
}

unsigned Model::getStoppedTasksCount() const
{
    return stoppedTasksCount;
}

unsigned Model::getZombieTasksCount() const
{
    return zombieTasksCount;
}

double Model::getUserspaceCPUUsageTime() const
{
    return userspaceCPUUsageTime;
}

double Model::getKernelspaceCPUUsageTime() const
{
    return kernelspaceCPUUsageTime;
}

double Model::getNIProcessesCPUUsageTime() const
{
    return NIProcessesCPUUsageTime;
}

double Model::getIdleCPUTime() const
{
    return idleCPUTime;
}

double Model::getIOCPUUsageTime() const
{
    return IOCPUUsageTime;
}

double Model::getHardwareInterruptsCPUUsageTime() const
{
    return hardwareInterruptsCPUUsageTime;
}

double Model::getSoftwareInterruptsCPUUsageTime() const
{
    return softwareInterruptsCPUUsageTime;
}

double Model::getStealCPUUsageTime() const
{
    return stealCPUUsageTime;
}

unsigned Model::getFreeMemorySize() const
{
    return freeMemorySize;
}

unsigned Model::getUsedMemorySize() const
{
    return usedMemorySize;
}

unsigned Model::getCacheMemorySize() const
{
    return cacheMemorySize;
}

unsigned Model::getFreeSwapSize() const
{
    return freeSwapSize;
}

unsigned Model::getUsedSwapSize() const
{
    return usedSwapSize;
}

unsigned Model::getAvailableMemorySize() const
{
    return availableMemorySize;
}

const std::vector<const ProcessT *> &Model::getProcesses() const
{
    return processes;
}

Model::~Model()
{
    clear();
}
