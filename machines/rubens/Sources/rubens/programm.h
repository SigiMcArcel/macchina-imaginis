#pragma once
#include <cAlsa.h>

namespace rubens
{
	enum enProgrammCommands
	{
		none = -1,
		on,
		off,
		pause,
		end,
		repeat
	};
	enum enProgrammMode
	{
		repeat,
		end
	};

	typedef struct ProgramStep_t
	{
		enProgrammCommands cmd;
		unsigned char  note = 0;
		unsigned char channel = 0;
		int counter = 0;
	};

	class Programm
	{
	private:
		


	};
}

