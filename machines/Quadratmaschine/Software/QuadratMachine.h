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

class QuadratMachine
    :public micomponents::miPhoneNumberInterface
    ,public miutils::EventListener
    ,public micomponents::miButtonEventInterface
{

    typedef enum class LighGameState_e
    {
        off,
        game1,
        game2
    }LighGameState;

private:
    const int _ComponentManagerRootIntervall = 5;
    const int _ComponentIntervall = 10;
    mimodule::ModuleManager _ModuleManager;
    mimodule::ModuleGecon32Input _GeconIn1;
    mimodule::ModuleGecon32Input _GeconIn2;
    mimodule::ModuleGecon32Output _GeconOut3;
    mimodule::ModuleGecon32Output _GeconOut4;
    mimodule::ModuleMiPhoneNumber _PhoneNumber;
    mimodule::ModuleMiRpiGpio _PhoneJack;
    mimodule::ModuleMiSevenSegment _Sevenofnine;
    mimodule::ModuleMiPotentiometerADS1115 _ModulVolume;

    micomponents::miComponentManager _MiComponentManager;

    std::string _WavePath;
    misound::Audio _Audio;
    std::shared_ptr<micomponents::miAudio> _MiAudio;
    std::shared_ptr<micomponents::miSevenSegment> _MiSevenSegment;
    std::shared_ptr<micomponents::miLedStrip> _MiLedStripYellow;
    std::shared_ptr<micomponents::miLedStrip> _MiLedStripBlue;
    std::shared_ptr<micomponents::miLedStrip> _MiLedStripRed;
    

    miutils::Timer _Timer;
    LighGameState _LighGameState;
    int _SegmentNumber;
   
    const int  MAX_LED_RED = 129;
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
    
    virtual void eventOccured(void* sender, const std::string& name) override;
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
        bool loop
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
    
public:
    QuadratMachine(const std::string& wavePath)
        : _ModuleManager(10)
        ,_GeconIn1("/dev/ttyUSB0", 1, "geconIn1")
        ,_GeconIn2("/dev/ttyUSB0", 2, "geconIn2")
        ,_GeconOut3("/dev/ttyUSB0", 3, "geconOut3")
        ,_GeconOut4("/dev/ttyUSB0", 4, "geconOut4")
        ,_PhoneNumber(5, "phone", 17, 27)
        ,_PhoneJack("PhoneJack", 
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
        ,_Sevenofnine("/dev/spidev0.0", "sevenofnine")
        ,_ModulVolume(0x48, 0.1, "Volume")

        , _MiComponentManager(5)
        , _WavePath(wavePath)
        , _Audio(std::string("plug:dmix0"), _WavePath)
        , _Timer("LightGame", this)
        , _LighGameState(LighGameState::off)
        , _SegmentNumber(0)
    {
       
        _ModuleManager.addModule(&_GeconIn1);
        _ModuleManager.addModule(&_GeconIn2);
        _ModuleManager.addModule(&_GeconOut3);
        _ModuleManager.addModule(&_GeconOut4);
        _ModuleManager.addModule(&_PhoneNumber);
        _ModuleManager.addModule(&_PhoneJack);
        _ModuleManager.addModule(&_Sevenofnine);
        _ModuleManager.addModule(&_ModulVolume);
        createComponents();
    }

    void start()
    {
        _ModuleManager.start();
        _MiComponentManager.start();
    }

    void stop()
    {
        _ModuleManager.stop();
    }

    virtual void PhoneNumberchanged(int number)
    {
        _MiSevenSegment->setSegment(_SegmentNumber, number);
        _SegmentNumber++;
        if (_SegmentNumber > 9)
        {
            _SegmentNumber = 0;
        }
    }


};