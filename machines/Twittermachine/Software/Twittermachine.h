#pragma once
#include <mi/micomponents/miPlayWaveButtonLamp.h>
#include <mi/micomponents/miAudio.h>
#include <mi/micomponents/miComponentBase.h>
#include <mi/mimodules/ModuleHoerterInput.h>
#include <mi/mimodules/ModuleHoerterOutput.h>
#include <mi/mimodules/ModuleMiPotentiometerADS115.h>
#include <mi/misound/Audio.h>
#include <mi/mimodules/ModuleManager.h>
#include <mi/micomponents/miComponentManager.h>
#include <mi/mitasks/miTaskManager.h>
#include <mi/mimodules/ModuleMiRpiGpio.h>

namespace mitwittermachine
{
    class TwitterMachine
    {

    private:
        int _ModuleManagerInterval; //50
        int _ComponentManagerIntervall; //5
        int _LampFlashIntervall; // 250

        mimodule::ModuleHoerterInput _HoerterIn1;
        mimodule::ModuleHoerterInput _HoerterIn2;
        mimodule::ModuleHoerterOutput _HoerterOut3;
        mimodule::ModuleHoerterOutput _HoerterOut4;
        mimodule::ModuleMiPotentiometerADS1115 _PotentiometerModule;
        mimodule::ModuleMiRpiGpio _PhoneJack;
     
        std::string _WavePath;
        misound::Audio _Audio;
        std::shared_ptr<micomponents::miAudio> _MiAudio;

        micomponents::miComponentManager _MiComponentManager;

        mitasks::miTaskBase _HoerterTask;
        mitasks::miTaskBase _AudioTask;
        mitasks::miTaskManager _TaskManager;

        void createAndAddWaveButtonLamp(
            const std::string& wavename,
            const std::string& inputChannelName,
            const std::string& outputChannelName,
            micomponents::ButtonType buttonType,
            micomponents::LampType lampType,
            bool loop
        );

        void createComponents();
        int getModulAddressFromChannelName(const std::string& input);
        int getValueFromIniFile(const std::string& filePath, const std::string& key);


    public:
        TwitterMachine(const std::string& wavePath, const std::string& iniPath)
            : _ModuleManagerInterval(10)
            , _ComponentManagerIntervall(1)
            , _LampFlashIntervall(250)
            , _HoerterIn1(0x21, "hoerterIn1")
            , _HoerterIn2(0x39, "hoerterIn1")
            , _HoerterOut3(0x20, "hoerterOut3")
            , _HoerterOut4(0x38, "hoerterOut4")
            , _PotentiometerModule(0x48, 0.1,5000.0, "Volume")
            , _PhoneJack("PhoneJack",
                std::vector<mimodule::ModuleMiRpiGpioConfiguration>
                {
                    {
                        sizeof(mimodule::ModuleMiRpiGpioConfiguration),
                            mimodule::ModuleMiRpiGpioState::Active,
                            mimodule::ModulChannelDirection::Input,
                            5
                    }
                }
            )
            , _WavePath(wavePath)
            , _Audio(std::string("plug:dmix0"), _WavePath,0.0,misound::VolumeTranspose::linear)
            , _MiComponentManager()
            , _HoerterTask("HoerterTask", 30, 0, 50, miutils::SchedulerType::Fifo)
            , _AudioTask("AudioTask", 20, 0, 20, miutils::SchedulerType::Other)
            , _TaskManager()
        {
          
            createComponents();

            //prepare Tasks
            _HoerterTask.addIOModul(&_HoerterIn1);
            _HoerterTask.addIOModul(&_HoerterIn2);
            _HoerterTask.addIOModul(&_HoerterOut3);
            _HoerterTask.addIOModul(&_HoerterOut4);
            _HoerterTask.addComponent(&_MiComponentManager);
          
            _AudioTask.addIOModul(&_PotentiometerModule);
            _AudioTask.addIOModul(&_PhoneJack);

            _TaskManager.AddTask(&_HoerterTask);
            _TaskManager.AddTask(&_AudioTask);

        }

        void start();
        void stop();
    };
}