#include "processesinfo.h"

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

const std::regex ProcessesInfo::pattern { R"(\s*(\d+)\s*(\d+.\d+)\s*(\d+.\d+)\s*(\d+:\d+:\d+)\n?)" };

std::unordered_map<unsigned, std::unique_ptr<ProcessesInfo::ProcessT>> ProcessesInfo::processesInfo;
//
#include <iostream>
//
void ProcessesInfo::parseProcessInfo(const std::string& process_info)
{
    if (std::regex_match(process_info, pattern)) {

        ProcessT* processT = new ProcessT;
        std::string regex_str;

        regex_str = std::regex_replace(process_info, pattern, "$2");
        regex_str[regex_str.find('.')] = ',';
        processT->cpu_usage = std::stod(regex_str);
        regex_str = std::regex_replace(process_info, pattern, "$3");
        regex_str[regex_str.find('.')] = ',';
        processT->memory_usage = std::stod(regex_str);
        processT->time = std::regex_replace(process_info, pattern, "$4");

        unsigned pid = std::stoul(std::regex_replace(process_info, pattern, "$1"));

        processesInfo[pid] = std::unique_ptr<ProcessT>(processT);
    }
}

void ProcessesInfo::update()
{
    processesInfo.clear();

    int pipefd[2];
    if (pipe(pipefd) == -1)
        return;

    pid_t pid = fork();
    if (pid == -1)
        return;

    if (pid == 0) {

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execl("/bin/ps" ,"ps", "-A", "-o", "pid,%cpu,%mem,time", (char*)0);
        exit(EXIT_SUCCESS);
    }
    else {

        close(pipefd[1]);
        wait(0);

        FILE* file = fdopen(pipefd[0], "r");

        if (file) {

            char* line = 0;
            size_t len = 0;

            while (getline(&line, &len, file) != -1)
                parseProcessInfo(line);

            free(line);
            fclose(file);
        }

        close(pipefd[0]);
    }
}

double ProcessesInfo::getCPUUsageByPID(unsigned pid)
{
    return processesInfo[pid]->cpu_usage;
}

double ProcessesInfo::getMemoryUsageByPID(unsigned pid)
{
    return processesInfo[pid]->memory_usage;
}

const std::string& ProcessesInfo::getTimeByPID(unsigned pid)
{
    return processesInfo[pid]->time;
}
