#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <map>
#include <vector>
#include <functional>

struct Process
{
    int in[2];
    int out[2];
    pid_t pid;
};

class Communicator
{
private:
    std::map<std::string, Process> processes;

public:
    const std::map<std::string, Process>& getProcesses() { return processes; }

    void CreateProcess(const std::string& name, const std::function<void(Process&)>& processFunc);
    void KillProcess(const std::string& name);
    void CheckProcessStatuses();
    bool CheckForChildOutput(std::string& outstring, const std::string& procname);
    bool WriteToChildInput(const std::string& message, const std::string& procname);
};
