#include <memory>
#include "QuadratMachine.h"

void miQuadratMachine::QuadratMachine::ButtonDown(const std::string& name)
{
	printf("QuadratMachine::ButtonDown %s\n", name.c_str());
	if (name == "emergencyStop_Button")
	{
		_MiComponentManager.disableInputsAll(true);
		_MiComponentManager.disableOutputsAll(true);
		_MiComponentManager.disableOutputs("Error", false);
		_MiComponentManager.LampOn("Error");
	}
	else if (name == "lampCheck")
	{
		_MiComponentManager.checkAll(true);
	}
	else if (name == "ledStripYellowOnOff_Button")
	{
		_MiLedStripYellow->startLED();
	}
	else if (name == "ledStripBlueOnOff_Button")
	{
		_MiLedStripBlue->startLED();
	}
	else if (name == "ledStripRedOnOff_Button")
	{
		_MiLedStripRed->startLED();
	}
	else if (name == "s18p18_Button")
	{
		_MiLedStripRed->startSmoothingLed(true);
	}
	else if (name == "error_Button")
	{
		_MiComponentManager.checkAll(true);
	}
	else if (name == "lightgame1")
	{
		_MiLedStripYellow->stopLED();
		_MiLedStripBlue->stopLED();
		_MiLedStripRed->stopLED();
		_LightGame.startGame(miQuadratMachine::LighGameState::game1);
	}
	else if (name == "lightgame2")
	{
		_MiLedStripYellow->stopLED();
		_MiLedStripBlue->stopLED();
		_MiLedStripRed->stopLED();
		_LightGame.startGame(miQuadratMachine::LighGameState::game2);
	}
	else
	{
		;
	}
}

void miQuadratMachine::QuadratMachine::ButtonUp(const std::string& name)
{
	printf("QuadratMachine::ButtonUp %s\n", name.c_str());
	if (name == "emergencyStop_Button")
	{
		_MiComponentManager.disableInputsAll(false);
		_MiComponentManager.disableOutputsAll(false);
		_MiComponentManager.LampOff("Error");
	}
	else if (name == "lampCheck")
	{
		_MiComponentManager.checkAll(false);
	}
	else if (name == "ledStripYellowOnOff_Button")
	{
		_MiLedStripYellow->stopLED();
	}
	else if (name == "ledStripBlueOnOff_Button")
	{
		_MiLedStripBlue->stopLED();
	}
	else if (name == "ledStripRedOnOff_Button")
	{
		_MiLedStripRed->stopLED();
	}
	else if (name == "s18p18_Button")
	{
		_MiLedStripRed->startSmoothingLed(false);
	}
	else if ((name == "lightgame1") || (name == "lightgame2"))
	{
		_LightGame.stopGame();
		
		if (_MiComponentManager.getComponentValue("ledStripYellowOnOff_Button", "ButtonState") == "1")
		{
			_MiLedStripYellow->startLED();
		}
		if (_MiComponentManager.getComponentValue("ledStripBlueOnOff_Button", "ButtonState") == "1")
		{
			_MiLedStripBlue->startLED();
		}
		if (_MiComponentManager.getComponentValue("ledStripRedOnOff_Button", "ButtonState") == "1")
		{
			_MiLedStripRed->startLED();
		}
	}
	else
	{
		;
	}
}

void miQuadratMachine::QuadratMachine::ButtonClick(const std::string& name)
{
	printf("QuadratMachine::ButtonClick %s\n", name.c_str());
	if (name == "ledStripeModeYellow")
	{
		_MiLedStripYellow->stepMode();
	}
	else if (name == "ledStripeModeBlue")
	{
		_MiLedStripBlue->stepMode();
	}
	else if (name == "ledStripeModeRed")
	{
		_MiLedStripRed->stepMode();
	}
}

void miQuadratMachine::QuadratMachine::ButtonToggle(bool state, const std::string& name)
{
	printf("QuadratMachine::ButtonClick %s\n", name.c_str());
}

void miQuadratMachine::QuadratMachine::PhoneNumberchanged(int number)
{
	_SegmentNumber++;
	_MiSevenSegment->setSegment(_SegmentNumber, number);

	if (_SegmentNumber > 8)
	{
		_SegmentNumber = 0;
		_MiSevenSegment->reset();
		_MiComponentManager.LampOff("p14");
	}
	else if (_SegmentNumber > 0)
	{
		_MiComponentManager.LampOn("p14");
	}
}

void miQuadratMachine::QuadratMachine::createAndAddWaveButtonLamp(
	const std::string& wavename,
	const std::string& inputChannelName,
	const std::string& outputChannelName,
	micomponents::ButtonType buttonType,
	micomponents::LampType lampType,
	bool loop,
	miButtonEventInterface* buttonEvent
)
{
	mimodule::ModuleChannel* inchannel = nullptr;
	mimodule::ModuleChannel* outchannel = nullptr;

	int InModulAdr = getModulAddressFromChannelName(inputChannelName);
	int OutModulAdr = getModulAddressFromChannelName(outputChannelName);
	
	if (InModulAdr == _GeconIn1.getAddress())
	{
		inchannel = _GeconIn1.getChannel(inputChannelName);
	}
	if (InModulAdr == _GeconIn2.getAddress())
	{
		inchannel = _GeconIn2.getChannel(inputChannelName);
	}
	if (OutModulAdr == _GeconOut3.getAddress())
	{
		outchannel = _GeconOut3.getChannel(outputChannelName);
	}
	if (OutModulAdr == _GeconOut4.getAddress())
	{
		outchannel = _GeconOut4.getChannel(outputChannelName);
	}
	
	if (inchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", inputChannelName.c_str());
	}
	
	if (outchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", outputChannelName.c_str());
		
	}

	micomponents::miPlayWaveButtonLamp* wbl =_MiComponentManager.registerComponent<micomponents::miPlayWaveButtonLamp>(
		wavename,
		-1,
		lampType,
		_LampFlashIntervall,
		inchannel,
		outchannel,
		buttonType,
		buttonEvent,
		_Audio,
		false,
		loop
		).get();
	_ModbusTask.addComponent(wbl);
}

void miQuadratMachine::QuadratMachine::createAndAddButtonLamp(
	const std::string& name,
	const std::string& inputChannelName,
	const std::string& outputChannelName,
	micomponents::ButtonType buttonType,
	micomponents::LampType lampType
)
{
	mimodule::ModuleChannel* inchannel;
	mimodule::ModuleChannel* outchannel;

	int InModulAdr = getModulAddressFromChannelName(inputChannelName);
	int OutModulAdr = getModulAddressFromChannelName(outputChannelName);
	
	if (InModulAdr == _GeconIn1.getAddress())
	{
		inchannel = _GeconIn1.getChannel(inputChannelName);
	}
	if (InModulAdr == _GeconIn2.getAddress())
	{
		inchannel = _GeconIn2.getChannel(inputChannelName);
	}
	if (OutModulAdr == _GeconOut3.getAddress())
	{
		outchannel = _GeconOut3.getChannel(outputChannelName);
	}
	if (OutModulAdr == _GeconOut4.getAddress())
	{
		outchannel = _GeconOut4.getChannel(outputChannelName);
	}

	if (inchannel == nullptr)
	{
		printf("createAndAddButtonLamp invalid channel %s\n", inputChannelName.c_str());
		return;
	}

	if (outchannel == nullptr)
	{
		printf("createAndAddButtonLamp invalid channel %s\n", outputChannelName.c_str());
		return;
	}
	
	micomponents::miButtonLamp* bl = _MiComponentManager.registerComponent<micomponents::miButtonLamp>(
		name,
		-1,
		lampType,
		_LampFlashIntervall,
		inchannel,
		outchannel,
		this,
		buttonType,
		false).get();
	_ModbusTask.addComponent(bl);
}

void miQuadratMachine::QuadratMachine::createAndAddLamp(
	const std::string& name,
	const std::string& outputChannelName,
	micomponents::LampType lampType
)
{
	mimodule::ModuleChannel* outchannel;

	int OutModulAdr = getModulAddressFromChannelName(outputChannelName);
	if (OutModulAdr == -1)
	{
		return;
	}
	if (OutModulAdr == _GeconOut3.getAddress())
	{
		outchannel = _GeconOut3.getChannel(outputChannelName);
	}
	if (OutModulAdr == _GeconOut4.getAddress())
	{
		outchannel = _GeconOut4.getChannel(outputChannelName);
	}
	if (outchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", outputChannelName.c_str());
		return;
	}
	micomponents::miLamp* l = _MiComponentManager.registerComponent<micomponents::miLamp>(
		name,
		lampType,
		-1,
		_LampFlashIntervall,
	outchannel).get();
	_ModbusTask.addComponent(l);
}

void miQuadratMachine::QuadratMachine::createAndAddButton(
	const std::string& name,
	const std::string& inputChannelName
)
{
	mimodule::ModuleChannel* inchannel;
	int InModulAdr = getModulAddressFromChannelName(inputChannelName);
	
	if (InModulAdr == -1)
	{
		return;
	}
	if (InModulAdr == _GeconIn1.getAddress())
	{
		inchannel = _GeconIn1.getChannel(inputChannelName);
	}
	if (InModulAdr == _GeconIn2.getAddress())
	{
		inchannel = _GeconIn2.getChannel(inputChannelName);
	}
	micomponents::miButton* b = _MiComponentManager.registerComponent<micomponents::miButton>(
		name,
		-1,
		inchannel,
		this,
		false).get();

	_ModbusTask.addComponent(b);
}

void miQuadratMachine::QuadratMachine::createComponents()
{

	createAndAddWaveButtonLamp("s1p1", "E1.0", "A3.0", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s2p2", "E1.1", "A3.1", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s3p3", "E1.2", "A3.2", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s4p4", "E1.3", "A3.3", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s5p5", "E1.4", "A3.4", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s6p6", "E1.5", "A3.5", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s7p7", "E1.6", "A3.6", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	//createAndAddWaveButtonLamp("s8p8", "E1.7", "A3.7", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true); //defect
	createAndAddWaveButtonLamp("s9p9", "E1.8", "A3.8", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s10p10", "E1.9", "A3.9", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s11p11", "E1.10", "A3.10", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s12p12", "E1.11", "A3.11", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s13p13", "E1.12", "A3.12", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s14p14", "E2.8", "A4.8", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddLamp("p14", "A3.13", micomponents::LampType::Fix);
	createAndAddWaveButtonLamp("s15p15", "E1.14", "A3.14", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s16p16", "E1.15", "A3.15", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s17p17", "E1.16", "A3.16", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s18p18", "E1.17", "", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true,this);
	createAndAddWaveButtonLamp("s19p19", "E1.18", "A3.18", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s20p20", "E1.19", "A3.19", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s21p21", "E1.20", "A3.20", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s22p22", "E1.21", "A3.21", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s23p23", "E1.22", "A3.22", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s24p24", "E1.23", "A3.23", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s25p25", "E1.24", "A3.24", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s26p26", "E1.25", "A3.25", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("s27p27", "E1.26", "A3.26", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s28p28", "E1.27", "A3.27", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s29p29", "E1.28", "A3.28", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s30p30", "E1.29", "A3.29", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s31p31", "E1.30", "A3.30", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s32p32", "E1.31", "A3.31", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s33p33", "E2.0", "A4.0", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddWaveButtonLamp("s34p34", "E2.1", "A4.1", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddWaveButtonLamp("s35p35", "E2.2", "A4.2", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButton("lightgame1", "E2.5");
	createAndAddButton("lightgame2", "E2.6");
	createAndAddButtonLamp("emergencyStop", "E2.7", "A4.7",micomponents::ButtonType::Switch, micomponents::LampType::Flash);
	createAndAddButton("ledStripeModeYellow", "E2.11");
	createAndAddButton("ledStripeModeBlue", "E2.10");
	createAndAddButton("ledStripeModeRed", "E2.9");
	createAndAddButton("lampCheck", "E2.13");
	createAndAddButtonLamp("ledStripYellowOnOff", "E2.14", "A4.14", micomponents::ButtonType::Switch, micomponents::LampType::Fix);
	createAndAddButtonLamp("ledStripBlueOnOff", "E2.15", "A4.15", micomponents::ButtonType::Switch, micomponents::LampType::Fix);
	createAndAddButtonLamp("ledStripRedOnOff", "E2.16", "A4.16", micomponents::ButtonType::Switch, micomponents::LampType::Fix);
	createAndAddLamp("Error", "A4.25", micomponents::LampType::Flash);
	
	_MiComponentManager.registerComponent<micomponents::miSevenSegment>("SevenSegmentComponent", 20, _SevenSegmentModule);
	_MiComponentManager.registerComponent<micomponents::miPhoneNumber>("RotaryDialComponent", 5, _RotaryDialModule.getChannel("PhoneNumber"), this);
	_MiSevenSegment = _MiComponentManager.getComponent<micomponents::miSevenSegment>("SevenSegmentComponent");

	_RotaryDialTask.addComponent(_MiComponentManager.getComponent<micomponents::miComponentBase>("RotaryDialComponent").get());
	_RotaryDialTask.addComponent(_MiComponentManager.getComponent<micomponents::miComponentBase>("SevenSegmentComponent").get());

	_MiComponentManager.registerComponent<micomponents::miLedStrip>("LedstripYellow", MAX_LED_YELLOW, _LedStripIntervall,-1, _SmoothLedIntervall, "/dev/ttyACM1");
	_MiComponentManager.registerComponent<micomponents::miLedStrip>("LedstripBlue", MAX_LED_BLUE, _LedStripIntervall, -1, _SmoothLedIntervall,"/dev/ttyACM2");
	_MiComponentManager.registerComponent<micomponents::miLedStrip>("LedstripRed", MAX_LED_RED, _LedStripIntervall, SMOOTHED_LED, _SmoothLedIntervall,"/dev/ttyACM0");
	_MiLedStripYellow = _MiComponentManager.getComponent<micomponents::miLedStrip>("LedstripYellow");
	_MiLedStripBlue = _MiComponentManager.getComponent<micomponents::miLedStrip>("LedstripBlue");
	_MiLedStripRed = _MiComponentManager.getComponent<micomponents::miLedStrip>("LedstripRed");
	_MiLedStripYellow->setMode(micomponents::LedStripMode::running);
	_MiLedStripBlue->setMode(micomponents::LedStripMode::running);
	_MiLedStripRed->setMode(micomponents::LedStripMode::running);

	_LedStripTask.addComponent(_MiComponentManager.getComponent<micomponents::miComponentBase>("LedstripYellow").get());
	_LedStripTask.addComponent(_MiComponentManager.getComponent<micomponents::miComponentBase>("LedstripBlue").get());
	_LedStripTask.addComponent(_MiComponentManager.getComponent<micomponents::miComponentBase>("LedstripRed").get());

	_MiComponentManager.registerComponent<micomponents::miAudio>(
		"miAudio",
		-1,
		_PotentiometerModule.getChannel("Potentiometer"),
		_PhoneJack.getChannel("GPIO5"),
		_Audio,
		100.0,
		"plug:dmix0",
		"plug:dmix1");
	_MiAudio = _MiComponentManager.getComponent<micomponents::miAudio>("miAudio");
	_AudioTask.addComponent(_MiComponentManager.getComponent<micomponents::miComponentBase>("miAudio").get());
}

int miQuadratMachine::QuadratMachine::getModulAddressFromChannelName(const std::string& input)
{
	// Finde die Position des ersten Punktes
	size_t pos = input.find('.');

	// Wenn ein Punkt gefunden wurde, gib den Teilstring davor zurück
	if (pos != std::string::npos) {
		return std::stoi(input.substr(1, pos));
	}
	else {
		// Wenn kein Punkt gefunden wurde, gib den gesamten String zurück
		return -1;
	}
}

void miQuadratMachine::QuadratMachine::start()
{
	_TaskManager.Start();
}

void miQuadratMachine::QuadratMachine::stop()
{
	_TaskManager.Stop();
}
