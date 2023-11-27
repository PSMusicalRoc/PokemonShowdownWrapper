#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#include "Communicator.hpp"

void CreatePokemonShowdownProcess(Process& proc)
{
    char* const argv[] = {
        // "zsh",
        // "-c",
        // "echo $PATH",
        "node",
        "vendor/pokemon-showdown/pokemon-showdown",
        "simulate-battle",
        NULL
    };

    execvp("node", argv);

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
            // comm.KillProcess("Showdown");
        }
    }

    std::cout << std::endl;

    return 0;
}
