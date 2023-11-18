#include "Rubens.h"


using namespace rubens2;

void Rubens::ButtonDown(const std::string& name)
{
	if (name == "emergencyStop")
	{
		_StartProgram = false;
		_ButtonLampManager.emergencyStopAll(true);
		_Leonardo.text("Emergency stop");
	}
	else if(name == "lampControl")
	{
		_ButtonLampManager.lampControlAll(true);
		_Leonardo.text("Lamp control");
	}
	else if  (name == "program")
	{
		_ProgramStep = 0;
		_StartProgram = true;
	}
	else
	{


	}
}
void Rubens::ButtonUp(const std::string& name)
{
	if (name == "emergencyStop")
	{
		_StartProgram = false;
		_Leonardo.run();
		_ButtonLampManager.emergencyStopAll(false);
	}
	else if (name == "lampControl")
	{
		_ButtonLampManager.lampControlAll(false);
		_Leonardo.run();
	}
	else if (name == "program")
	{
		_ProgramStep = 0;
		_StartProgram = false;
	}
	else
	{


	}
}
void Rubens::ButtonClick(const std::string& name)
{

}
void Rubens::ButtonToggle(bool state, const std::string& name)
{

}

void Rubens::addProgrammEntry(const std::string& name, Cmd_e cmd)
{
	RubensProgramm p(name, cmd);
	_ProgramList.push_back(p);
}

void rubens2::Rubens::potValueChanged(int val)
{
	_Volume.setVolume(val);
}

void Rubens::createProgramm()
{
	addProgrammEntry("A7", Cmd_e::on);
	addProgrammEntry("A7", Cmd_e::pause);
	addProgrammEntry("A7", Cmd_e::off);
	addProgrammEntry("A8", Cmd_e::on);
	addProgrammEntry("A8", Cmd_e::pause);
	addProgrammEntry("A8", Cmd_e::off);
	addProgrammEntry("A6", Cmd_e::on);
	addProgrammEntry("A6", Cmd_e::pause);

	addProgrammEntry("A6", Cmd_e::off);
	addProgrammEntry("A5", Cmd_e::on);
	addProgrammEntry("A5", Cmd_e::pause);
	addProgrammEntry("A5", Cmd_e::off);
	addProgrammEntry("A14", Cmd_e::on);
	addProgrammEntry("A14", Cmd_e::pause);
	addProgrammEntry("A15", Cmd_e::off);
	addProgrammEntry("A15", Cmd_e::pause);
	addProgrammEntry("A15", Cmd_e::off);
	addProgrammEntry("A22", Cmd_e::on);
	addProgrammEntry("A22", Cmd_e::pause);

	addProgrammEntry("A22", Cmd_e::off);
	addProgrammEntry("A24", Cmd_e::on);
	addProgrammEntry("A24", Cmd_e::pause);
	addProgrammEntry("A24", Cmd_e::off);
	addProgrammEntry("A31", Cmd_e::on);
	addProgrammEntry("A30", Cmd_e::on);
	addProgrammEntry("A14", Cmd_e::pause);
	addProgrammEntry("A31", Cmd_e::off);
	addProgrammEntry("A30", Cmd_e::off);
	addProgrammEntry("A34", Cmd_e::on);
	addProgrammEntry("A30", Cmd_e::pause);
	addProgrammEntry("A34", Cmd_e::off);
	addProgrammEntry("A36", Cmd_e::on);
	addProgrammEntry("A38", Cmd_e::on);
	addProgrammEntry("A30", Cmd_e::pause);
	addProgrammEntry("A38", Cmd_e::off);
	addProgrammEntry("A44", Cmd_e::on);
	addProgrammEntry("A30", Cmd_e::pause);
	addProgrammEntry("A44", Cmd_e::off);
}

// Geerbt über EventListener
void Rubens::eventOccured(void* sender, const std::string& name)
{
	if (_StartProgram)
	{
		RubensProgramm p = _ProgramList[_ProgramStep];
		if (p._Cmd == Cmd_e::pause)
		{
			_ProgramStep++;
			return;
		}
		if (p._Cmd == Cmd_e::on)
		{
			_ButtonLampManager.LampOn(p._Name);
		}
		if (p._Cmd == Cmd_e::off)
		{
			_ButtonLampManager.LampOff(p._Name);
		}
		
		if (_ProgramStep >= static_cast<int>(_ProgramList.size()))
		{
			_ProgramStep = 0;
		}
		_ProgramStep++;
	}
}


void Rubens::createAndAddButtonLamp(
	const std::string& wavename,
	const std::string& inputChannelName,
	const std::string& outputChannelName,
	micomponents::ButtonType buttonType,
	micomponents::LampType lampType,
	bool loop
)
{
	mimodule::ModuleChannel* inchannel = _DoepferInputChannel0.getChannel(inputChannelName);
	if (inchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", inputChannelName.c_str());
		return;
	}
	mimodule::ModuleChannel* outchannel = _DoepferOutputChannel0.getChannel(outputChannelName);
	if (outchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", outputChannelName.c_str());
		return;
	}
	micomponents::miPlayWaveButtonLamp* lamp = new micomponents::miPlayWaveButtonLamp(
		lampType,
		250,
		inchannel,
		outchannel,
		wavename,
		buttonType,
		false,
		_WavePath,
		false);

	_ButtonLamps.push_back(lamp);
	_ButtonLampManager.registerAll(lamp, lamp);
}

void Rubens::createAndAddLamps(
	const std::string& name,
	const std::string& outputChannelName
)
{
	mimodule::ModuleChannel* channel = _DoepferOutputChannel1.getChannel(outputChannelName);
	if (channel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", outputChannelName.c_str());
		return;
	}
	micomponents::miLamp* lamp = new micomponents::miLamp(
		micomponents::LampType::Fix,
		250,
		channel,
		name);
	_Lamps.push_back(lamp);
	_ButtonLampManager.registerLampControl(lamp);
}

void Rubens::createComponents()
{
	createAndAddButtonLamp("A1", "E1", "A1", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("A2", "E2", "A2", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P1", "E3", "A3", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P2", "E4", "A4", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A3", "E5", "A5", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P3", "E6", "A6", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P4", "E7", "A7", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P5", "E8", "A8", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A4", "E9", "A9", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P7", "E10", "A10", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P8", "E11", "A11", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A5", "E12", "A12", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P6", "E13", "A13", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P9", "E14", "A14", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A6", "E15", "A15", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P10", "E16", "A16", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A7", "E17", "A17", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P13", "E18", "A18", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A9", "E19", "A19", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P12", "E20", "A20", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A8", "E21", "A21", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P11", "E22", "A22", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P14", "E23", "A23", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A10", "E24", "A24", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P19", "E25", "A25", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P15", "E26", "A26", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P16", "E27", "A27", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P17", "E28", "A28", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P18", "E29", "A29", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A15", "E30", "A30", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P20", "E31", "A31", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A12", "E32", "A32", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P21", "E33", "A33", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A13", "E34", "A34", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P22", "E35", "A35", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P23", "E36", "A36", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P25", "E37", "A37", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A14", "E38", "A38", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P26", "E39", "A39", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P27", "E40", "A40", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P28", "E41", "A41", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P31", "E42", "A42", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A16", "E43", "A43", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("P30", "E44", "A44", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P29", "E45", "A45", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P33", "E48", "A48", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P34", "E49", "A49", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("P35", "E50", "A50", micomponents::ButtonType::Switch, micomponents::LampType::Fix, true);
	createAndAddButtonLamp("A17", "E51", "A51", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);
	createAndAddButtonLamp("A11", "E51", "A51", micomponents::ButtonType::PushButtonToggle, micomponents::LampType::Flash, false);


	//lamps
	createAndAddLamps("A2", "A2");
	createAndAddLamps("A3", "A3");
	createAndAddLamps("A4", "A4");
	createAndAddLamps("A5", "A5");
	createAndAddLamps("A6", "A6");
	createAndAddLamps("A7", "A7");
	createAndAddLamps("A8", "A8");
	createAndAddLamps("A9", "A9");
	createAndAddLamps("A10", "A10");
	createAndAddLamps("A11", "A11");
	createAndAddLamps("A12", "A12");
	createAndAddLamps("A13", "A13");
	createAndAddLamps("A14", "A14");
	createAndAddLamps("A15", "A15");
	createAndAddLamps("A16", "A16");
	createAndAddLamps("A17", "A17");
	createAndAddLamps("A18", "A18");
	createAndAddLamps("A19", "A19");
	createAndAddLamps("A20", "A20");
	createAndAddLamps("A21", "A21");
	createAndAddLamps("A22", "A22");
	createAndAddLamps("A23", "A23");
	createAndAddLamps("A24", "A24");
	createAndAddLamps("A25", "A25");
	createAndAddLamps("A26", "A26");
	createAndAddLamps("A27", "A27");
	createAndAddLamps("A28", "A28");
	createAndAddLamps("A29", "A29");
	createAndAddLamps("A30", "A30");
	createAndAddLamps("A31", "A31");
	createAndAddLamps("A32", "A32");
	createAndAddLamps("A33", "A33");
	createAndAddLamps("A34", "A34");
	createAndAddLamps("A35", "A35");
	createAndAddLamps("A36", "A36");
	createAndAddLamps("A37", "A37");
	createAndAddLamps("A38", "A38");
	createAndAddLamps("A39", "A39");
	createAndAddLamps("A40", "A40");
	createAndAddLamps("A41", "A41");
	createAndAddLamps("A42", "A42");
	createAndAddLamps("A43", "A43");

	//notaus
	mimodule::ModuleChannel* inchannel = _DoepferInputChannel0.getChannel("E47");
	if (inchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", "E47");
		return;
	}
	mimodule::ModuleChannel* outchannel = _DoepferOutputChannel0.getChannel("A47");
	if (outchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", "A47");
		return;
	}
	micomponents::miButtonLamp* emergencystop = new micomponents::miButtonLamp(
		micomponents::LampType::Flash,
		250,
		inchannel,
		outchannel,
		this,
		"emergencyStop",
		micomponents::ButtonType::Switch
	);
	_ButtonLampManager.registerLampControl(emergencystop);

	inchannel = _DoepferInputChannel0.getChannel("E46");
	if (inchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", "E46");
		return;
	}
	outchannel = _DoepferOutputChannel0.getChannel("A46");
	if (outchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", "A46");
		return;
	}
	micomponents::miButtonLamp* lampcontrol = new micomponents::miButtonLamp(
		micomponents::LampType::Flash,
		250,
		inchannel,
		outchannel,
		this,
		"lampControl",
		micomponents::ButtonType::Switch
	);
	_ButtonLampManager.registerLampControl(lampcontrol);


	inchannel = _DoepferInputChannel0.getChannel("E64");
	if (inchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", "E64");
		return;
	}
	outchannel = _DoepferOutputChannel0.getChannel("A63");
	if (outchannel == nullptr)
	{
		printf("createAndAddLamps invalid channel %s\n", "A63");
		return;
	}
	micomponents::miButtonLamp* programm = new micomponents::miButtonLamp(
		micomponents::LampType::Fix,
		250,
		inchannel,
		outchannel,
		this,
		"program",
		micomponents::ButtonType::Switch
	);
	_ButtonLampManager.registerLampControl(programm);
}
