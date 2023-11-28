#include "QuickReading.hpp"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void QuickRead(std::string& outputbuf, int fd)
{
    char buffer[1024];
    outputbuf = "";
    
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);
    
    while (true) {
        ssize_t len;

        // Use select() to check if data is available to be read from out_pipe[0]
        int retval;
        timeval timeout = { 0, 100 };
        retval = select(fd + 1, &read_set, NULL, NULL, &timeout);

        // std::cout << "Num Ready : " << num_ready << std::endl;

        if (retval > 0) {
            // Data is available to be read
            len = read(fd, buffer, sizeof(buffer));
            //std::cout << len << std::endl;
            if (len > 0) {
                buffer[len] = '\0';
                outputbuf += buffer;
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
            std::cerr << "select() error!" << std::endl;
            break;
        }
    }
}