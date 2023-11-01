#include <cstdio>
#include <sys/mount.h>
#include <mi/micomponents/miPlayWaveButtonLamp.h>
#include <mi/mimodules/ModuleHoerterInput.h>
#include <mi/mimodules/ModuleHoerterOutput.h>
#include <mi/mimodules/ModuleMiPotentiometer.h>
#include <mi/micomponents/miVolume.h>
#include <mi/mimodules/ModuleManager.h>
#include <mi/misound/Alsa.h>

int main(int argc, char* argv[])
{
    int handle = -1;
    if (argc == 2)
    {

        if (::strcmp(argv[1], "-d") == 0) {
            printf("miwoodenmachine daemonize\n");
            if (::daemon(1, 1)) {
                perror("Unable to daemonize");
                return 1;
            }
        }
    }

    fprintf(stderr,"miwoodenmachine\n");
    
    handle = ::open("/dev/sda1", O_RDWR);
    if (handle == -1)
    {
        printf("miwoodenmachine failed -> no usb\n");
    }
    ::close(handle);
    if (mount("/dev/sda1", "/home/root/sounds", "vfat", MS_NOATIME, NULL)) {
        if (errno == EBUSY) {
            printf("miwoodenmachine Mountpoint busy\n");
            return 1;
        }
        else {
            printf("miwoodenmachine Mount error: %s\n", strerror(errno));
            return 1;
        }
    }
    else {
        printf("miwoodenmachine Mount successful\n");
    }
    const std::string _WavePath = std::string("/home/root/sounds");

   
    mimodule::ModuleHoerterInput Module1(0x38, "Module1");
    mimodule::ModuleHoerterInput Module2(0x39, "Module2");
    mimodule::ModuleHoerterInput Module3(0x3A, "Module3");
    mimodule::ModuleHoerterOutput Module4(0x20, "Module4");
    mimodule::ModuleHoerterOutput Module5(0x21, "Module5");
    mimodule::ModuleHoerterOutput Module6(0x22, "Module6");
    mimodule::ModuleMiPotentiometer ModulVolume(0x42, 3, "Volume");

    micomponents::miVolume volume(ModulVolume.getChannel("Potentiometer"),20,100);
    micomponents::miPlayWaveButtonLamp lamp01(micomponents::LampType::Flash, 250, Module1.getChannel("E1"), Module4.getChannel("A1"), "P1", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp02(micomponents::LampType::Flash, 250, Module1.getChannel("E2"), Module4.getChannel("A2"), "P1", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp03(micomponents::LampType::Flash, 250, Module1.getChannel("E3"), Module4.getChannel("A3"), "P2", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp04(micomponents::LampType::Flash, 250, Module1.getChannel("E4"), Module4.getChannel("A4"), "P3", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp05(micomponents::LampType::Flash, 250, Module1.getChannel("E5"), Module6.getChannel("A5"), "P4", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp06(micomponents::LampType::Flash, 250, Module1.getChannel("E6"), Module4.getChannel("A6"), "P5", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp07(micomponents::LampType::Flash, 250, Module1.getChannel("E7"), Module4.getChannel("A7"), "P6", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp08(micomponents::LampType::Flash, 250, Module1.getChannel("E8"), Module4.getChannel("A8"), "P7", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp09(micomponents::LampType::Flash, 250, Module2.getChannel("E1"), Module6.getChannel("A6"), "P8", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp10(micomponents::LampType::Flash, 250, Module2.getChannel("E2"), Module5.getChannel("A2"), "P9", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp11(micomponents::LampType::Flash, 250, Module2.getChannel("E3"), Module5.getChannel("A3"), "P10", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp12(micomponents::LampType::Flash, 250, Module2.getChannel("E4"), Module5.getChannel("A4"), "P11", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp13(micomponents::LampType::Flash, 250, Module2.getChannel("E5"), Module5.getChannel("A5"), "P12", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp14(micomponents::LampType::Flash, 250, Module2.getChannel("E6"), Module5.getChannel("A6"), "P13", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp15(micomponents::LampType::Flash, 250, Module2.getChannel("E7"), Module5.getChannel("A7"), "P14", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp16(micomponents::LampType::Flash, 250, Module2.getChannel("E8"), Module5.getChannel("A8"), "P15", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp17(micomponents::LampType::Flash, 250, Module3.getChannel("E1"), Module6.getChannel("A1"), "P16", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp18(micomponents::LampType::Flash, 250, Module3.getChannel("E2"), Module6.getChannel("A2"), "P17", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp19(micomponents::LampType::Flash, 250, Module3.getChannel("E3"), Module6.getChannel("A3"), "P18", micomponents::ButtonType::Switch, false, _WavePath, true);
    micomponents::miPlayWaveButtonLamp lamp20(micomponents::LampType::Flash, 250, Module3.getChannel("E4"), Module6.getChannel("A7"), "P19", micomponents::ButtonType::Switch, false, _WavePath, true);

    mimodule::ModuleManager man(50);
    man.addModule(&Module1);
    man.addModule(&Module2);
    man.addModule(&Module3);
    man.addModule(&Module4);
    man.addModule(&Module5);
    man.addModule(&Module6);
    man.addModule(&ModulVolume);
    man.start();
    while (true)
    {
        usleep(100000);
    }
    return 0;
}