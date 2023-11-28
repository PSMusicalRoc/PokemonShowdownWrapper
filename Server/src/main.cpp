#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#include "Communicator.hpp"
#include "ShowdownOutputSplitter.hpp"

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
    std::vector<std::string> messages = {
        ">start {\"formatid\":\"gen5randombattle\"}\n",
        ">player p1 {\"name\":\"Roc\"}\n",
        ">player p2 {\"name\":\"AI\"}\n"
    };
    int messagesSent = 0;

    comm.WriteToChildInput(messages[messagesSent], "Showdown");
    messagesSent++;

    while (!comm.getProcesses().empty())
    {
        comm.CheckProcessStatuses();

        if (!comm.IsProcessRunning("PlayerClient"))
        {
            comm.KillProcess("Showdown");
        }

        if (comm.CheckForChildOutput(procout, "Showdown"))
        {
            std::cout << procout;
            
            std::string p1Write;
            SplitShowdownOutput(procout, &p1Write);
            comm.WriteToChildInput(p1Write, "PlayerClient");

            if (messagesSent < messages.size())
            {
                comm.WriteToChildInput(messages[messagesSent], "Showdown");
                messagesSent++;
            }
        }

        if (comm.CheckForChildOutput(procout, "PlayerClient"))
        {
            std::cout << "Player 1 Wrote Back!" << std::endl;
            std::cout << procout << std::endl;
        }
    }

    std::cout << std::endl;

    return 0;
}
