#pragma once
#include <BWAPI.h>
#include <memory>
#include "SquadUnit.h"
#include "Common.h"

// Handles single units in a squad. Micro, target decision (focusfire) and positioning

namespace BlackCrow {

	class BlackCrow;
	class SquadUnit;

	class Squad {
	public:

		Squad();
		void onFrame();

		std::vector<SquadUnitPtr> units;

		void add(const SquadUnitPtr unit);
		void remove(const SquadUnitPtr unit);
		void moveAll(BWAPI::Position position, bool queue);


	protected:
		BlackCrow &bc;
		Squad(BlackCrow &parent);
	};

	class ScoutSquad : public Squad {
	public:
		ScoutSquad(BlackCrow &parent);
		void onFrame();
		bool isStillScouting();
		int locationsToScout();
		std::vector<BWAPI::TilePosition> getScoutingPositions();
		void addScoutPosition(BWAPI::TilePosition position);
		void addStartLocations();
		void addExpansions();
		void setGlobalSearch(bool searchGlobally);

	private:
		bool globalSearch;
		std::vector<BWAPI::TilePosition> scoutLocations;
	};

	class AttackSquad : public Squad {
	public:
		AttackSquad(BlackCrow &parent);
		void onFrame();
	};

	class StallSquad : public Squad {
	public:
	};
}