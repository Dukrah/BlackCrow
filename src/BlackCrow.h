#pragma once
#include <BWAPI.h>
#include <iostream>
#include <assert.h>
#include <chrono>
#include "Macro.h"
#include "Strategy.h"
#include "Map.h"
#include "Debug.h"
#include "Enemy.h"
#include "Config.h"
#include "Builder.h"
#include "Util.h"
#include "Tech.h"
#include "Army.h"
#include "PathFinder.h"


// Remember not to use "Broodwar" in any global class constructor!
namespace BlackCrow {
	
	class BlackCrow {
	public:
		void onStart();
		void onEnd(bool isWinner);
		void onFrame();
		void onSendText(std::string text);
		void onReceiveText(BWAPI::Player player, std::string text);
		void onPlayerLeft(BWAPI::Player player);
		void onNukeDetect(BWAPI::Position target);
		void onUnitDiscover(BWAPI::Unit unit);
		void onUnitEvade(BWAPI::Unit unit);
		void onUnitShow(BWAPI::Unit unit);
		void onUnitHide(BWAPI::Unit unit);
		void onUnitCreate(BWAPI::Unit unit);
		void onUnitDestroy(BWAPI::Unit unit);
		void onUnitMorph(BWAPI::Unit unit);
		void onUnitRenegade(BWAPI::Unit unit);
		void onSaveGame(std::string gameName);
		void onUnitComplete(BWAPI::Unit unit);

		BWEM::Map &bwem;
		Builder builder;
		Config config;
		Debug debug;
		Enemy enemy;
		Macro macro;
		Map map;
		Strategy strategy;
		Tech tech;
		Army army;
		PathFinder pathFinder;

		BlackCrow();
		void init();
		bool isExecutingCommandFrame();
		double getAverageFrameTime();
		
	private:
		std::chrono::high_resolution_clock::time_point lastFrame;
		Util::DampenedAverager frameTimeAverager;
	};
}