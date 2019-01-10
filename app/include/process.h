#ifndef PROCESS_H
#define PROCESS_H

#include <string>

struct process {

    unsigned pid { 0 };
    std::string user;
    int priority { 0 };
    int nice_value { 0 };
    unsigned virtual_memory { 0 };
    unsigned resident_memory { 0 };
    unsigned shared_memory { 0 };
    char status { 'S' };
    double cpu_usage { 0 };
    double memory_usage { 0 };
    std::string time;
    std::string command;
};

using ProcessT = struct process;

#endif // PROCESS_H
