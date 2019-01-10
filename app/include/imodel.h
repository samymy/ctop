#ifndef IMODEL_H
#define IMODEL_H

#include <string>
#include <vector>
#include "process.h"

class IModel {

public:
    virtual void readProcInfo() = 0;
    virtual const std::string& getCurrentTime() const = 0;
    virtual const std::string& getRunningTime() const = 0;
    virtual unsigned getUserSessions() const = 0;
    virtual double getLoadAverageOverOneMinute() const = 0;
    virtual double getLoadAverageOverFiveMinutes() const = 0;
    virtual double getLoadAverageOverFifteenMinutes() const = 0;
    virtual unsigned getRunningTasksCount() const = 0;
    virtual unsigned getSleepingTasksCount() const = 0;
    virtual unsigned getStoppedTasksCount() const = 0;
    virtual unsigned getZombieTasksCount() const = 0;
    virtual double getUserspaceCPUUsageTime() const = 0;
    virtual double getKernelspaceCPUUsageTime() const = 0;
    virtual double getNIProcessesCPUUsageTime() const = 0; // NIProcesses - processes with a manually set "nice" value
    virtual double getIdleCPUTime() const = 0;
    virtual double getIOCPUUsageTime() const = 0;
    virtual double getHardwareInterruptsCPUUsageTime() const = 0;
    virtual double getSoftwareInterruptsCPUUsageTime() const = 0;
    virtual double getStealCPUUsageTime() const = 0;
    virtual unsigned getFreeMemorySize() const = 0;
    virtual unsigned getUsedMemorySize() const = 0;
    virtual unsigned getCacheMemorySize() const = 0;
    virtual unsigned getFreeSwapSize() const = 0;
    virtual unsigned getUsedSwapSize() const = 0;
    virtual unsigned getAvailableMemorySize() const = 0;
    virtual const std::vector<const ProcessT*>& getProcesses() const = 0;
    virtual ~IModel() = default;
};

#endif // IMODEL_H
