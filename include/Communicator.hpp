#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <map>
#include <vector>

struct Process
{
    FILE* in;
    FILE* out;
    pid_t pid;
};

class Communicator
{
private:
    std::map<std::string, Process> processes;

public:
    const std::map<std::string, Process>& getProcesses() { return processes; }

    void CreateProcess(const std::string& name, const std::vector<std::string>& processargs);
    void CheckProcessStatuses();
};
