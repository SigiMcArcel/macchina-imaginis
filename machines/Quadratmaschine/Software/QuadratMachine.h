#pragma once
#include <mi/micomponents/miPlayWaveButtonLamp.h>
#include <mi/micomponents/miAudio.h>
#include <mi/micomponents/miSevenSegment.h>
#include <mi/micomponents/miLedStrip.h>
#include <mi/micomponents/miPhoneNumber.h>
#include <mi/micomponents/miComponentBase.h>
#include <mi/mimodules/ModuleGecon32Input.h>
#include <mi/mimodules/ModuleGecon32Output.h>
#include <mi/mimodules/ModuleMiPotentiometerADS115.h>
#include <mi/mimodules/ModuleMiRpiGpio.h>
#include <mi/misound/Audio.h>
#include <mi/mimodules/ModuleManager.h>
#include <mi/micomponents/miComponentManager.h>
#include <mi/mitasks/miTaskManager.h>
#include "LightGame.h"

namespace miQuadratMachine
{
    class QuadratMachine
        :public micomponents::miPhoneNumberInterface
        , public micomponents::miButtonEventInterface
    {

    private:
        int _ModuleManagerInterval; //50
        int _RotaryDialModuleIntervall; //5
        int _SevenSegmentModuleIntervall; //20
        int _PotentiometerModuleIntervall;
        int _PhoneJackModuleIntervall;

        int _ComponentManagerIntervall; //5
        int _LedStripIntervall; //100
        int _SmoothLedIntervall; // 100
        int _LightGameIntervall; // 100
        int _LampFlashIntervall; // 250
        double _VolumeOffset;

        mimodule::ModuleGecon32Input _GeconIn1;
        mimodule::ModuleGecon32Input _GeconIn2;
        mimodule::ModuleGecon32Output _GeconOut3;
        mimodule::ModuleGecon32Output _GeconOut4;
        mimodule::ModuleMiPhoneNumber _RotaryDialModule;
        mimodule::ModuleMiRpiGpio _PhoneJack;
        mimodule::ModuleMiSevenSegment _SevenSegmentModule;
        mimodule::ModuleMiPotentiometerADS1115 _PotentiometerModule;
      
        std::string _WavePath;
        misound::Audio _Audio;
        std::shared_ptr<micomponents::miAudio> _MiAudio;
        std::shared_ptr<micomponents::miSevenSegment> _MiSevenSegment;
        std::shared_ptr<micomponents::miLedStrip> _MiLedStripYellow;
        std::shared_ptr<micomponents::miLedStrip> _MiLedStripBlue;
        std::shared_ptr<micomponents::miLedStrip> _MiLedStripRed;
        LightGame _LightGame;

        micomponents::miComponentManager _MiComponentManager;

        mitasks::miTaskBase _ModbusTask;
        mitasks::miTaskBase _LedStripTask;
        mitasks::miTaskBase _RotaryDialTask;
        mitasks::miTaskBase _SevenSegmentTask;
        mitasks::miTaskBase _AudioTask;
        mitasks::miTaskManager _TaskManager;

        int _SegmentNumber;

        const int  MAX_LED_RED = 130;
        const int  SMOOTHED_LED = 129;
        const int  MAX_LED_YELLOW = 132;
        const int  MAX_LED_BLUE = 148;

        //To do Would be nice to give a name
        const std::vector<mimodule::ModuleMiRpiGpioConfiguration> _PhoneJackConfiguration =
        {
            {
                sizeof(mimodule::ModuleMiRpiGpioConfiguration),
                mimodule::ModuleMiRpiGpioState::Active,
                mimodule::ModulChannelDirection::Input,
                5
            }
        };

        virtual void ButtonDown(const std::string& name);
        virtual void ButtonUp(const std::string& name);
        virtual void ButtonClick(const std::string& name);
        virtual void ButtonToggle(bool state, const std::string& name);

        void createAndAddWaveButtonLamp(
            const std::string& wavename,
            const std::string& inputChannelName,
            const std::string& outputChannelName,
            micomponents::ButtonType buttonType,
            micomponents::LampType lampType,
            bool loop,
            miButtonEventInterface* buttonEvent = nullptr
        );

        void createAndAddButtonLamp(
            const std::string& name,
            const std::string& inputChannelName,
            const std::string& outputChannelName,
            micomponents::ButtonType buttonType,
            micomponents::LampType lampType
        );

        void createAndAddLamp(
            const std::string& name,
            const std::string& outputChannelName,
            micomponents::LampType lampType
        );

        void createAndAddButton(
            const std::string& name,
            const std::string& inputChannelName
        );
        void createComponents();
        int getModulAddressFromChannelName(const std::string& input);

        virtual void PhoneNumberchanged(int number);

        template <typename T>
        T getValueFromIniFile
        (const std::string& filePath, const std::string& key, const T& defaultValue = T())
        {
            std::ifstream iniFile(filePath);
            if (!iniFile.is_open()) {
                std::cerr << "Could not open " << filePath << std::endl;
                return defaultValue; // Rückgabe des Standardwerts im Fehlerfall
            }

            std::string line;
            while (std::getline(iniFile, line)) {
                // Trim leading and trailing whitespace
                line.erase(0, line.find_first_not_of(" \t\n\r"));
                line.erase(line.find_last_not_of(" \t\n\r") + 1);

                // Überspringe Kommentare oder leere Zeilen
                if (line.empty() || line[0] == ';' || line[0] == '#') {
                    continue;
                }

                // Suche nach dem Schlüssel (Key)
                if (line.find(key + "=") == 0) {
                    // Extrahiere den Wert hinter dem "="
                    std::string valueStr = line.substr(line.find("=") + 1);

                    std::istringstream iss(valueStr);
                    T value;
                    if (iss >> value) { // Konvertiere den Wert in den gewünschten Typ T
                        iniFile.close();
                        return value;
                    }
                    else {
                        std::cerr << "Fehler: Ungültiger Wert für den Schlüssel: " << key << std::endl;
                        iniFile.close();
                        return defaultValue;
                    }
                }
            }

            iniFile.close();
            std::cerr << "Fehler: Schlüssel " << key << " nicht gefunden." << std::endl;
            return defaultValue; // Rückgabe des Standardwerts, wenn der Schlüssel nicht gefunden wurde
        }

    public:
        QuadratMachine(const std::string& wavePath, const std::string& iniPath)
            : _ModuleManagerInterval(getValueFromIniFile<int>(iniPath, "ModuleManagerInterval", 10))
            , _RotaryDialModuleIntervall(getValueFromIniFile<int>(iniPath, "RotaryDialModuleIntervall",5 ))//5
            , _SevenSegmentModuleIntervall(getValueFromIniFile<int>(iniPath, "SevenSegmentModuleIntervall", 20)) //20
            , _PotentiometerModuleIntervall(getValueFromIniFile<int>(iniPath, "PotentiometerModuleIntervall", 20)) //20
            , _PhoneJackModuleIntervall(getValueFromIniFile<int>(iniPath, "PhoneJackModuleIntervall", 10)) //10
            , _ComponentManagerIntervall(getValueFromIniFile<int>(iniPath, "ComponentManagerIntervall", 10)) //1
            , _LedStripIntervall(getValueFromIniFile<int>(iniPath, "LedStripIntervall", 100)) //100
            , _SmoothLedIntervall(getValueFromIniFile<int>(iniPath, "SmoothLedIntervall", 2)) // 2
            , _LightGameIntervall(getValueFromIniFile<int>(iniPath, "LightGameIntervall", 100)) // 100
            , _LampFlashIntervall(getValueFromIniFile<int>(iniPath, "LampFlashIntervall", 250)) //250
            , _VolumeOffset(getValueFromIniFile<double>(iniPath, "VolumeOffset", 25.0)) // 25
            , _GeconIn1("/dev/ttyUSB0", 1, "geconIn1")
            , _GeconIn2("/dev/ttyUSB0", 2, "geconIn2")
            , _GeconOut3("/dev/ttyUSB0", 3, "geconOut3")
            , _GeconOut4("/dev/ttyUSB0", 4, "geconOut4")
            , _RotaryDialModule(_RotaryDialModuleIntervall, "phone", 17, 27)
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

            , _SevenSegmentModule("/dev/spidev0.0", "sevenofnine")
            , _PotentiometerModule(0x48, 0.1, "Volume")
            , _WavePath(wavePath)
            , _Audio(std::string("plug:dmix0"), _WavePath, _VolumeOffset)
            , _LightGame(&_MiComponentManager,_LightGameIntervall)
            , _MiComponentManager()
            , _ModbusTask("ModbusTask",30,0,50,miutils::SchedulerType::Fifo)
            , _LedStripTask("LedStripTask", 5, 0, 20, miutils::SchedulerType::Fifo)
            , _RotaryDialTask("PhoneNumberTask", 5, 0, 20, miutils::SchedulerType::Fifo)
            , _SevenSegmentTask("SevenSegmentTask", 20, 0, 20, miutils::SchedulerType::Other)
            , _AudioTask("AudioTask", 20, 0, 20, miutils::SchedulerType::Other)
            , _TaskManager()
            , _SegmentNumber(0)
        {
            
            createComponents();

            //prepare Tasks
            _ModbusTask.addIOModul(&_GeconIn1);
            _ModbusTask.addIOModul(&_GeconIn2);
            _ModbusTask.addIOModul(&_GeconOut3);
            _ModbusTask.addIOModul(&_GeconOut4);
            _ModbusTask.addComponent(&_LightGame);
            _ModbusTask.addComponent(&_MiComponentManager);
            _LedStripTask.addComponent(&_MiComponentManager);
           
            _RotaryDialTask.addIOModul(&_RotaryDialModule);
            _SevenSegmentTask.addIOModul(&_SevenSegmentModule);
            _AudioTask.addIOModul(&_PotentiometerModule);
            _AudioTask.addIOModul(&_PhoneJack);

            _TaskManager.AddTask(&_ModbusTask);
            _TaskManager.AddTask(&_LedStripTask);
            _TaskManager.AddTask(&_RotaryDialTask);
            _TaskManager.AddTask(&_SevenSegmentTask);
            _TaskManager.AddTask(&_AudioTask);

        }

        void start();
        void stop();
    };
}