#include <cstdio>
#include <string>
#include <sys/mount.h>
#include "QuadratMachine.h"

int main(int argc, char* argv[])
{
    std::string soundPath("/usr/share/ misounds");

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
    if (argc == 3)
    {
        soundPath = argv[1];
    }

    fprintf(stderr,"mi quadrat machine\n");
    
    const std::string _WavePath = soundPath;
    miQuadratMachine::QuadratMachine _QuadratMachine(_WavePath,"");
    _QuadratMachine.start();

    while (true)
    {
        usleep(100000);
    }
    return 0;
}