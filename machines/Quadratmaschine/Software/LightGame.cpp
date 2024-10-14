#include "LightGame.h"

void miQuadratMachine::LightGame::startGame(miQuadratMachine::LighGameState state)
{
	_LampCount = _MiComponentManager->getLampCount();
	_LightGameLamp = 0;
	_LighGameState = state;
	_Running = true;
}

void miQuadratMachine::LightGame::stopGame()
{
	_LighGameState = LighGameState::shutdown;
}

bool miQuadratMachine::LightGame::componentProcess(int rootInterval, long tick)
{
	switch (_LighGameState)
	{
	case LighGameState::off:
	{
		_Running = false;
		_LightGameLamp = 0;
		break;
	}
	case LighGameState::shutdown:
	{
		_MiComponentManager->LampOverrideAll(false);
		_LighGameNext = LighGameState::off;
		_LighGameState = LighGameState::wait;
		break;
	}
	case LighGameState::game1:
	{
		_MiComponentManager->LampOverrideAll(true);
		_MiComponentManager->LampOffAll();
		_LighGameNext = LighGameState::game1play;
		_LighGameState = LighGameState::wait;
		break;
	}
	case LighGameState::game1play:
	{
		if (_LightGameLamp > 0)
		{
			_MiComponentManager->LampOffNumberIntern(_LightGameLamp - 1);
		}
		_MiComponentManager->LampOnNumberIntern(_LightGameLamp);
		if (_LightGameLamp > _LampCount)
		{
			_LightGameLamp = -1;
		}
		_LightGameLamp++;

		break;
	}
	case LighGameState::game2:
	{
		_MiComponentManager->LampOverrideAll(true);
		_MiComponentManager->LampOffAll();
		_LighGameNext = LighGameState::game2play;
		_LighGameState = LighGameState::wait;
		break;
	}
	case LighGameState::game2play:
	{
		if (!_LightGameToggle)
		{
			_LightGameToggle = true;
			_MiComponentManager->LampOnAllIntern();
		}
		else
		{
			_LightGameToggle = false;
			_MiComponentManager->LampOffAllIntern();
		}
		break;
	}
	case LighGameState::wait:
	{
		_LighGameState = _LighGameNext;
		break;
	}
	}

	return true;
}
