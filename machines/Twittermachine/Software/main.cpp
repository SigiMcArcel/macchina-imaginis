#include <cstdio>
#include <sys/mount.h>
#include "Twittermachine.h"

int main(int argc, char* argv[])
{
    if (argc == 2)
    {

        if (::strcmp(argv[1], "-d") == 0) {
            printf("mi quadrat nachine daemonize\n");
            if (::daemon(1, 1)) {
                perror("Unable to daemonize");
                return 1;
            }
        }
    }

    fprintf(stderr,"mi quadrat machine\n");
    
    const std::string _WavePath = std::string("/home/root/sounds");
    mitwittermachine::TwitterMachine _TwitterMachine(_WavePath,"/home/root/quadrat.ini");
    _TwitterMachine.start();

    while (true)
    {
        usleep(100000);
    }
    return 0;
}