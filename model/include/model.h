#ifndef MODEL_H
#define MODEL_H

#include "imodel.h"

class Model: public IModel {

    std::string currentTime;
    std::string runningTime;
    unsigned userSessions;
    double loadAverageOverOneMinute;
    double loadAverageOverFiveMinutes;
    double loadAverageOverFifteenMinutes;
    unsigned runningTasksCount;
    unsigned sleepingTasksCount;
    unsigned stoppedTasksCount;
    unsigned zombieTasksCount;
    double userspaceCPUUsageTime;
    double kernelspaceCPUUsageTime;
    double NIProcessesCPUUsageTime;
    double idleCPUTime;
    double IOCPUUsageTime;
    double hardwareInterruptsCPUUsageTime;
    double softwareInterruptsCPUUsageTime;
    double stealCPUUsageTime;
    unsigned freeMemorySize;
    unsigned usedMemorySize;
    unsigned cacheMemorySize;
    unsigned freeSwapSize;
    unsigned usedSwapSize;
    unsigned availableMemorySize;
    std::vector<const ProcessT*> processes;

    const ProcessT *getProcessByPID(unsigned pid);

    void setCurrentTime();
    void setRunningTime();
    void setUserSessions();
    void setLoadAverages();
    void setTaskCounts();
    void setCPUUsageTime();
    void setMemorySizes();
    void setProcesses();

    void clear();

public:
    void readProcInfo() override;
    const std::string& getCurrentTime() const override;
    const std::string& getRunningTime() const override;
    unsigned getUserSessions() const override;
    double getLoadAverageOverOneMinute() const override;
    double getLoadAverageOverFiveMinutes() const override;
    double getLoadAverageOverFifteenMinutes() const override;
    unsigned getRunningTasksCount() const override;
    unsigned getSleepingTasksCount() const override;
    unsigned getStoppedTasksCount() const override;
    unsigned getZombieTasksCount() const override;
    double getUserspaceCPUUsageTime() const override;
    double getKernelspaceCPUUsageTime() const override;
    double getNIProcessesCPUUsageTime() const override;
    double getIdleCPUTime() const override;
    double getIOCPUUsageTime() const override;
    double getHardwareInterruptsCPUUsageTime() const override;
    double getSoftwareInterruptsCPUUsageTime() const override;
    double getStealCPUUsageTime() const override;
    unsigned getFreeMemorySize() const override;
    unsigned getUsedMemorySize() const override;
    unsigned getCacheMemorySize() const override;
    unsigned getFreeSwapSize() const override;
    unsigned getUsedSwapSize() const override;
    unsigned getAvailableMemorySize() const override;
    const std::vector<const ProcessT*>& getProcesses() const override;
    ~Model();
};


#endif // MODEL_H
