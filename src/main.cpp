#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#include "Communicator.hpp"

void CreatePokemonShowdownProcess(Process& proc)
{
    std::cout << "This will be the pokemon showdown process, i think" << std::endl;

    exit(0);
}

int main() {
    Communicator comm;
    
    comm.CreateProcess("Showdown", CreatePokemonShowdownProcess);

    std::string procout;

    while (!comm.getProcesses().empty())
    {
        comm.CheckProcessStatuses();
        if (comm.CheckForChildOutput(procout, "Showdown"))
        {
            std::cout << procout;
        }
    }

    std::cout << std::endl;

    return 0;
}
