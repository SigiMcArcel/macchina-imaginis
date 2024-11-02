#include <memory>
#include "Twittermachine.h"


void mitwittermachine::TwitterMachine::createAndAddWaveButtonLamp(
	const std::string& wavename,
	const std::string& inputChannelName,
	const std::string& outputChannelName,
	micomponents::ButtonType buttonType,
	micomponents::LampType lampType,
	bool loop
)
{
	mimodule::ModuleChannel* inchannel = nullptr;
	mimodule::ModuleChannel* outchannel = nullptr;

	int InModulAdr = getModulAddressFromChannelName(inputChannelName);
	int OutModulAdr = getModulAddressFromChannelName(outputChannelName);
	
	if (InModulAdr == _HoerterIn1.getAddress())
	{
		inchannel = _HoerterIn1.getChannel(inputChannelName);
	}
	if (InModulAdr == _HoerterIn2.getAddress())
	{
		inchannel = _HoerterIn2.getChannel(inputChannelName);
	}
	if (OutModulAdr == _HoerterOut3.getAddress())
	{
		outchannel = _HoerterOut3.getChannel(outputChannelName);
	}
	if (OutModulAdr == _HoerterOut4.getAddress())
	{
		outchannel = _HoerterOut4.getChannel(outputChannelName);
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
		nullptr,
		_Audio,
		false,
		loop
		).get();
	_HoerterTask.addComponent(wbl);
}

void mitwittermachine::TwitterMachine::createComponents()
{

	createAndAddWaveButtonLamp("a1", "E33.0", "A32.0", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a2", "E33.1", "A32.1", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a3", "E33.2", "A32.2", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a4", "E33.3", "A32.3", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a5", "E33.4", "A32.4", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a6", "E33.5", "A32.5", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a7", "E33.6", "A32.6", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a8", "E33.7", "A32.7", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a9", "E57.0", "A56.0", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a10", "E57.1", "A56.1", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a11", "E57.2", "A56.2", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a12", "E57.3", "A56.3", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a13", "E57.4", "A56.4", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a14", "E57.5", "A56.5", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a15", "E57.6", "A56.6", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	createAndAddWaveButtonLamp("a16", "E57.7", "A56.7", micomponents::ButtonType::Switch, micomponents::LampType::Flash, true);
	
	_MiComponentManager.registerComponent<micomponents::miAudio>(
		"miAudio",
		-1,
		_PotentiometerModule.getChannel("Potentiometer"),
		_PhoneJack.getChannel("GPIO5"),
		_Audio,
		100.0,
		"plug:dmix1",
		"plug:dmix0");
	_MiAudio = _MiComponentManager.getComponent<micomponents::miAudio>("miAudio");
	_AudioTask.addComponent(_MiComponentManager.getComponent<micomponents::miComponentBase>("miAudio").get());
}

int mitwittermachine::TwitterMachine::getModulAddressFromChannelName(const std::string& input)
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

void mitwittermachine::TwitterMachine::start()
{
	_TaskManager.Start();
}

void mitwittermachine::TwitterMachine::stop()
{
	_TaskManager.Stop();
}

int mitwittermachine::TwitterMachine::getValueFromIniFile(const std::string& filePath, const std::string& key)
{
	std::ifstream iniFile(filePath);
	if (!iniFile.is_open()) {
		std::cerr << "Coud not open " << filePath << std::endl;
		return -1; // oder eine andere geeignete Fehlerkennzeichnung
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

			// Konvertiere den String in eine Zahl
			try {
				int value = std::stoi(valueStr);
				iniFile.close();
				return value;
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Fehler: Ungültiger Wert für den Schlüssel: " << key << std::endl;
				iniFile.close();
				return -1;
			}
		}
	}

	iniFile.close();
	std::cerr << "Fehler: Schlüssel " << key << " nicht gefunden." << std::endl;
	return -1; // oder eine andere Fehlerkennzeichnung, wenn der Schlüssel nicht gefunden wurde
}