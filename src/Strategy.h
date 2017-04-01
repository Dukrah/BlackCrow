#pragma once
#include <BWAPI.h>
#include <queue>
#include "Squad.h"

namespace BlackCrow {

	class BlackCrow;
	class PlannedUnit;
	class BaseInformation;
	class ScoutSquad;
	class Area;
	class Planned;

	class Strategy {

	public:

		enum BuildOrder {
			NINE_POOL,
			OVERPOOL,
			TWELVE_HATCH
		};

		// Variables
		std::queue<BWAPI::UnitType> buildOrder;
		std::vector<ScoutSquad> scoutSquads;
		BuildOrder bo;

		// General
		Strategy(BlackCrow &parent);
		void onStart();
		void onFrame();
		void onUnitDiscovered(BWAPI::Unit unit);
		void onUnitDestroyed(BWAPI::Unit unit);
		void dynamicDecision();

	private:
		BlackCrow &bc;

		// Buildorder
		BuildOrder getStartBuildOrder();
		void fillBuildOrder(BuildOrder build);
		void fillBuildOrderItem(BWAPI::UnitType item);
		void followBuildOrder();

		
		void startInitialScout();
	};
}