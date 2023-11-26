#pragma once
#include <mi/micomponents/miPlayWaveButtonLamp.h>
#include <mi/mimodules/ModuleMidiInput.h>
#include <mi/mimodules/ModuleMidiOutput.h>
#include <mi/mimodules/ModuleMiPotentiometer.h>
#include <mi/micomponents/miVolume.h>
#include <mi/mimodules/ModuleManager.h>
#include <mi/misound/Alsa.h>
#include <mi/micomponents/mibuttonLampMananger.h>
#include <mi/miutils/Timer.h>
#include <list>

#include "Leonardo.h"

namespace rubens2
{
	enum  struct Cmd_e
	{
		on,
		off,
		pause
	};

	struct RubensProgramm
	{

		
		std::string _Name;
		Cmd_e _Cmd;
		RubensProgramm(const std::string name, Cmd_e cmd)
			:_Name(name)
			, _Cmd(cmd)
		{

		}
	};


class Rubens :public micomponents::miButtonEventInterface, public miutils::EventListener, public Leonardo::PotChangedEvent
{
	
private:
	std::vector<micomponents::miPlayWaveButtonLamp*> _ButtonLamps;
	std::vector<micomponents::miLamp*> _Lamps;
	micomponents::mibuttonLampMananger _ButtonLampManager;

	

	mimodule::ModuleManager _ModuleManager;
	mimodule::ModuleMidiInput _DoepferInputChannel0;
	mimodule::ModuleMidiOutput _DoepferOutputChannel0;
	mimodule::ModuleMidiOutput _DoepferOutputChannel1;

	std::string _WavePath;
	miutils::Timer _Timer;

	misound::AlsaVolume _Volume;
	Leonardo _Leonardo;

	bool _StartProgram;
	int _ProgramStep;
	std::vector<RubensProgramm> _ProgramList;

	virtual void eventOccured(void* sender, const std::string& name) override;
	virtual void ButtonDown(const std::string& name);
	virtual void ButtonUp(const std::string& name);
	virtual void ButtonClick(const std::string& name);
	virtual void ButtonToggle(bool state, const std::string& name);


	void createAndAddButtonLamp(
		const std::string& wavename,
		const std::string& inputChannelName,
		const std::string& outputChannelName,
		micomponents::ButtonType buttonType,
		micomponents::LampType lampType,
		bool loop
	);
	
	void createAndAddLamps(
		const std::string& name,
		const std::string& outputChannelName
	);
	
	void createComponents();

	void createProgramm();
	void addProgrammEntry(const std::string& name, Cmd_e cmd);


public:
	Rubens(const std::string& wavePath)
	:_ButtonLampManager()
	, _ModuleManager(100)
	,_DoepferInputChannel0(0,"_DoepferInputChannel0")
	,_DoepferOutputChannel0(0, "_DoepferOutputChannel0")
	,_DoepferOutputChannel1(1, "_DoepferOutputChannel1")
	,_WavePath(wavePath)
	,_Timer("Program",this)
	, _Volume(20)
	,_Leonardo("/dev/ttyACM0",this)
	, _StartProgram(false)
	, _ProgramStep(0)
	{
		_ModuleManager.addModule(&_DoepferInputChannel0);
		_ModuleManager.addModule(&_DoepferOutputChannel0);
		_ModuleManager.addModule(&_DoepferOutputChannel1);
		createComponents();
		createProgramm();
		_Leonardo.run();
		_ButtonLampManager.LampOn("A37_1");
	}


	void start()
	{
		_ModuleManager.start();
	}

	void stop()
	{
		_ModuleManager.stop();
		_Timer.Stop();
	}



	// Geerbt über PotChangedEvent
	virtual void potValueChanged(int val) override;
	

};
}

