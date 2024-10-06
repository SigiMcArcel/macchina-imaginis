#include <cstdio>
#include <sys/mount.h>
#include "QuadratMachine.h"




int main(int argc, char* argv[])
{
   
#if 0
    int handle = -1;
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
    
    handle = ::open("/dev/sda1", O_RDWR);
    if (handle == -1)
    {
        printf("quadrat machine failed -> no usb\n");
    }
    ::close(handle);
    if (mount("/dev/sda1", "/home/root/sounds", "vfat", MS_NOATIME, NULL)) {
        if (errno == EBUSY) {
            printf("quadrat machine Mountpoint busy\n");
            return 1;
        }
        else {
            printf("quadrat machine Mount error: %s\n", strerror(errno));
            return 1;
        }
    }
    else {
        printf("miwoodenmachine Mount successful\n");
    }
#endif
    const std::string _WavePath = std::string("/home/root/sounds");
    QuadratMachine _QuadratMachine(_WavePath);
    _QuadratMachine.start();
    while (true)
    {
        usleep(100000);
    }
    return 0;
}