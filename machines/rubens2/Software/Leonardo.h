#pragma once
#include <string>
#include <mi/midriver/SerialDriver.h>

class Leonardo : public miDriver::SerialDriverStringReceivedEvent
{
public:
	class PotChangedEvent
	{
	public:
		virtual void potValueChanged(int val) = 0;
	};
private:
	miDriver::SerialDriver _Serial;
	std::string _DevPath;
	int _Fd;
	PotChangedEvent* _Event;
public:
	Leonardo(std::string devPath, PotChangedEvent* event)
		:_Serial(devPath,9600)
		,_DevPath(devPath)
		, _Event(event)
	{
		_Serial.serialRegister(this, "\n");
		open();
	}
	~Leonardo()
	{
		_Serial.close();
	}

	bool open()
	{
		_Serial.open();
		return true;
	}
	void close()
	{
		_Serial.close();
	}

	void run()
	{
		_Serial.serialWrite("RUN:\n");
	}

	void stop()
	{
		_Serial.serialWrite("STOP:\n");
	}


	void emergency()
	{
		_Serial.serialWrite("EMC:\n");
	}

	void lampControl()
	{
		_Serial.serialWrite("LC:\n");
	}

	
public:

	// Geerbt über SerialDriverStringReceivedEvent
	virtual void DataReceived(const std::string& data) override
	{
		if (_Event != nullptr)
		{
			if (data.find("POT:") == 0)
			{
				std::string tmp = data.substr(4, data.size() -2);
				
				long val = std::strtol(tmp.c_str(),nullptr, 10);
				
				_Event->potValueChanged(static_cast<int>(val));
			}
		}
	}
};

