#include "Communicator.hpp"
#include <csignal>
#include <exception>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

struct fork_error : public std::exception
{
    const char* what() { return "proc_pid < 0\n"; }
};

void Communicator::CreateProcess(const std::string& name, const std::vector<std::string>& processargs)
{
    pid_t proc_pid;
    proc_pid = fork();

    Process newproc;

    if (proc_pid < 0)
    {
        fork_error e;
        throw e;
    }
    else if (proc_pid == 0)
    {
        // child process
        for (int i = 0; i < 10; i++)
        {
            sleep(1);
            std::cout << "Child Process is printing for the " << i << "th time!" << std::endl;
        }
        exit(0);
    }
    else
    {
        std::cout << "This is the parent. Get back here, Thomas..." << std::endl;
        newproc.pid = proc_pid;
        processes.emplace(name, newproc);
    }
}


void Communicator::CheckProcessStatuses()
{
    for (auto it = processes.begin(); it != processes.end(); it++)
    {
        int status;
        pid_t result = waitpid(it->second.pid, &status, WNOHANG);
        if (result < 0)
        {
            std::cout << "PID " << it->second.pid << " encountered some error and crashed." << std::endl;
            auto temp = it;
            it--;
            processes.erase(temp);
        }
        else if (result > 0)
        {
            std::cout << "PID " << it->second.pid << " completed with status code " << status << std::endl;
            auto temp = it;
            it--;
            processes.erase(temp);
        }

        if (processes.empty())
            break;
    }
}
