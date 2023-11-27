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

void StartPlayerPort(Process& proc)
{
    char* const argv[] = {NULL};
    execvp("./player_client", argv);

    exit(0);
}

int main() {
    Communicator comm;
    
    comm.CreateProcess("Showdown", CreatePokemonShowdownProcess);
    comm.CreateProcess("PlayerClient", StartPlayerPort);

    std::string procout;

    comm.WriteToChildInput(">start {\"formatid\":\"gen5randombattle\"}\n", "Showdown");

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
