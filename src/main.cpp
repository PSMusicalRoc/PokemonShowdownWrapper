#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#include "Communicator.hpp"

int main() {
    Communicator comm;
    
    for (int i = 0; i < 15; i++)
    {
        comm.CreateProcess(std::to_string(i + 1), {});
        usleep(500000);
    }

    while (!comm.getProcesses().empty())
    {
        comm.CheckProcessStatuses();
    }

    return 0;
}
