#include "Communicator.hpp"
#include <csignal>
#include <exception>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

struct fork_error : public std::exception
{
    const char* what() { return "proc_pid < 0\n"; }
};

struct pipe_error : public std::exception
{
    const char* what() { return "pipe() < 0\n"; }
};

void Communicator::CreateProcess(const std::string& name, const std::function<void(Process&)>& processFunc)
{
    Process newproc;

    if (pipe(newproc.in) < 0)
    {
        throw pipe_error();
    }

    if (pipe(newproc.out) < 0)
    {
        throw pipe_error();
    }

    pid_t proc_pid;
    proc_pid = fork();

    if (proc_pid < 0)
    {
        throw fork_error();
    }
    else if (proc_pid == 0)
    {
        // child process
        close(newproc.in[1]);
        close(newproc.out[0]);

        dup2(newproc.in[0], STDIN_FILENO);
        dup2(newproc.out[1], STDOUT_FILENO);

        close(newproc.in[0]);
        close(newproc.out[1]);

        processFunc(newproc);
        exit(0);
    }
    else
    {
        // parent process
        std::cout << "This is the parent. Get back here, Thomas..." << std::endl;
        newproc.pid = proc_pid;

        close(newproc.out[1]);
        close(newproc.in[0]);

        processes.emplace(name, newproc);
    }
}

void Communicator::KillProcess(const std::string& name)
{
    if (processes.find(name) == processes.end())
        return;
    
    Process& proc = processes.at(name);
    kill(proc.pid, SIGKILL);
    processes.erase(name);
}



void Communicator::CheckProcessStatuses()
{
    for (auto it = processes.begin(); it != processes.end(); it++)
    {
        int status;
        pid_t result = waitpid(it->second.pid, &status, WNOHANG);
        if (result < 0)
        {
            std::cout << "Process " << it->first
                << " (" <<  it->second.pid
                << ") encountered some error and crashed." << std::endl;
            auto temp = it;
            it--;
            processes.erase(temp);
        }
        else if (result > 0)
        {
            std::cout << "Process " << it->first
                << " (" <<  it->second.pid
                << ") completed with status code " << status << std::endl;
            auto temp = it;
            it--;
            processes.erase(temp);
        }

        if (processes.empty())
            break;
    }
}

bool Communicator::CheckForChildOutput(std::string& outstring, const std::string& procname)
{
    if (processes.find(procname) == processes.end())
        return false;
    
    Process& proc = processes.at(procname);
    
    char buffer[1024];
    outstring = "";
    
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(proc.out[0], &read_set);
    
    while (true) {
        ssize_t len;

        // Use select() to check if data is available to be read from out_pipe[0]
        int retval;
        timeval timeout = { 0, 100 };
        retval = select(proc.out[0] + 1, &read_set, NULL, NULL, &timeout);

        // std::cout << "Num Ready : " << num_ready << std::endl;

        if (retval > 0) {
            // Data is available to be read
            len = read(proc.out[0], buffer, sizeof(buffer));
            //std::cout << len << std::endl;
            if (len > 0) {
                buffer[len] = '\0';
                outstring += buffer;
                if (len < 1024)
                    break;
            }
            else break;
        } else if (retval == 0) {
            // No data is available to be read
            //std::cout << "No data" << std::endl;
            break;
        } else {
            // Error occurred
            std::cout << "select() error!" << std::endl;
            break;
        }
    }

    return outstring != "";
}
