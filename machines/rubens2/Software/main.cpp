#include <cstdio>
#include <sys/mount.h>
#include "Rubens.h"

int main(int argc, char* argv[])
{
    const std::string _WavePath = std::string("/home/root/sounds");
    rubens2::Rubens rubens(_WavePath);

    int handle = -1;
    if (argc == 2)
    {

        if (::strcmp(argv[1], "-d") == 0) {
            printf("Rubens2 daemonize\n");
            if (::daemon(1, 1)) {
                perror("Unable to daemonize");
                return 1;
            }
        }
    }

    fprintf(stderr,"Rubens2\n");
    
    handle = ::open("/dev/sda1", O_RDWR);
    if (handle == -1)
    {
        printf("miwoodenmachine failed -> no usb\n");
    }
    ::close(handle);
    if (mount("/dev/sda1", "/home/root/sounds", "vfat", MS_NOATIME, NULL)) {
        if (errno == EBUSY) {
            printf("miwoodenmachine Mountpoint busy\n");
            handle = ::open("/dev/sda1", O_RDWR);
            if (handle == -1)
            {
                printf("miwoodenmachine failed -> no usb\n");
                return 1;
            }
        }
        else {
            printf("miwoodenmachine Mount error: %s\n", strerror(errno));
            return 1;
        }
    }
    else {
        printf("miwoodenmachine Mount successful\n");
    }
   
    rubens.start();

    while (true)
    {
        sleep(1);
    }
    return 0;
}