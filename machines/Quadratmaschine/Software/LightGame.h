#pragma once
#include <mi/micomponents/miComponentManager.h>
#include <mi/miutils/Time.h>
namespace miQuadratMachine
{
	typedef enum class LighGameState_e
	{
		off,
		shutdown,
		game1,
		game1play,
		game2,
		game2play,
		wait
	}LighGameState;

	class LightGame : public micomponents::miComponentBase
	{
		micomponents::miComponentManager* _MiComponentManager;
		LighGameState _LighGameState;
		LighGameState _LighGameNext;
		int _LightGameLamp;
		bool _LightGameToggle;
		int _LightGameIntervall;
		miutils::Time _GameTime;
		int _LampCount;
		bool _Running;
		
	public:
		LightGame(micomponents::miComponentManager* manager, int intervall)
			: miComponentBase("LightGame",intervall)
			, _MiComponentManager(manager)
			, _LighGameState(LighGameState::off)
			, _LightGameLamp(0)
			, _LightGameToggle(false)
			, _LightGameIntervall(intervall)
			, _GameTime()
			, _LampCount(0)
			, _Running(false)
		{

		}

		void startGame(miQuadratMachine::LighGameState state);
		void stopGame();
		virtual bool componentProcess(int rootInterval, long tick) override;
	};
}

