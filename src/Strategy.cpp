#include "Strategy.h"
#include "BlackCrow.h"
#include <BWEM/bwem.h>
#include "Area.h"
#include "Planned.h"
#include "Macro.h"


namespace BlackCrow {

	using namespace BWAPI;
	using namespace Filter;

	Strategy::Strategy(BlackCrow &parent) : bc(parent) {}

	void Strategy::onStart() {
		fillBuildOrder(getStartBuildOrder());
	}

	void Strategy::onFrame() {

		if (buildOrder.size() > 0) {
			followBuildOrder();
		} else {
			dynamicDecision();
		}

		if (bc.macro.getUsedSupply() >= 18 && scoutSquads.size() <= 0) {
			startInitialScout();
		}

		for (ScoutSquad& ss : scoutSquads) {
			ss.onFrame();
		}
	}

	void Strategy::followBuildOrder() {
		UnitType type = buildOrder.front();
		Resources unreservedResources = bc.macro.getUnreservedResources();

		if (unreservedResources.minerals >= type.mineralPrice() && unreservedResources.gas >= type.gasPrice()) {
			if (type.isBuilding()) {
				if (type == UnitTypes::Zerg_Extractor) {
					bc.macro.buildExtractor();
				} else if (type == UnitTypes::Zerg_Hatchery) {
					bc.macro.expand();
				} else {
					TilePosition buildPosition = bc.builder.getBuildingSpot(type, false);
					if (buildPosition != TilePositions::None)
						bc.macro.planBuilding(type, buildPosition);
				}
			} else {
				if (bc.macro.getUnreservedLarvaeAmount() > 0)
					bc.macro.planUnit(type, bc.macro.startPosition);
			}
			buildOrder.pop();
		}
	}

	void Strategy::dynamicDecision() {

		// Lets do the macro first
		if (bc.macro.getFreeSupply() >= UnitTypes::Zerg_Drone.supplyRequired() && bc.macro.getWorkersNeededForSaturation() - bc.macro.getTypeCurrentlyPlanned(UnitTypes::Zerg_Drone < 0)) {
			if (bc.macro.getUnreservedResources().minerals >= 50 && bc.macro.getUnreservedLarvaeAmount() > 0)
				bc.macro.buildWorkerDrone();
		}

		if (bc.macro.getFreeSupply() + bc.macro.getPlannedSupply() < UnitTypes::Zerg_Overlord.supplyProvided()) {
			if (bc.macro.getUnreservedResources().minerals >= 100 && bc.macro.getUnreservedLarvaeAmount() > 0)
				bc.macro.planUnit(UnitTypes::Zerg_Overlord, bc.macro.startPosition);
		}

		if (bc.macro.getUnreservedResources().minerals >= 300 && bc.macro.getTotalLarvaeAmount()<= 0) {
			if (bc.macro.getTypeCurrentlyPlanned(UnitTypes::Zerg_Hatchery) < 1) {

				TilePosition buildPosition = bc.builder.getBuildingSpot(UnitTypes::Zerg_Hatchery, false);
				if (buildPosition != TilePositions::None)
					bc.macro.planBuilding(UnitTypes::Zerg_Hatchery, buildPosition);
			}
		}
	}

	void Strategy::onUnitDiscovered(BWAPI::Unit unit) {
		
	}

	void Strategy::onUnitDestroyed(BWAPI::Unit unit) {

	}

	Strategy::BuildOrder Strategy::getStartBuildOrder() {
		if (Broodwar->enemy()->getRace().getName() == "Terran") {
			return Strategy::BuildOrder::TWELVE_HATCH;
		} else {
			if (Broodwar->enemy()->getRace().getName() == "Zerg") {
				return Strategy::BuildOrder::NINE_POOL;
			} else {
				if (Broodwar->enemy()->getRace().getName() == "Protoss") {
					return Strategy::BuildOrder::OVERPOOL;
				}
			}
		}
		return Strategy::BuildOrder::OVERPOOL;
	}


	void Strategy::startInitialScout() {
		ScoutSquad scoutSquad(bc);
		BWAPI::Unit worker = bc.macro.getDroneForBuilding(bc.macro.startPosition);
		if (worker) {
			SquadUnit su(worker);
			scoutSquad.add(su);

			scoutSquad.addStartLocations();
			scoutSquads.push_back(scoutSquad);
		}
	}

	void Strategy::fillBuildOrder(BuildOrder build) {

		switch (build) {
		case Strategy::BuildOrder::NINE_POOL:

			// Drone @4
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @5
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @6
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @7
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @8
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Spawning Pool @9
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Spawning_Pool);

			// Drone @8
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Extractor @9
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Extractor);

			// Overlord @8
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Overlord);

			// Drone @8
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// 6 Zerglings @8
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling);
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling);
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling);

			break;

		case Strategy::BuildOrder::OVERPOOL:

			// Drone @4
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @5
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @6
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @7
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @8
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Overlord @9
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Overlord);

			// Spawning Pool @9
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Spawning_Pool);

			// Drone @8
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @9
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @10
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);
			// Now has 11 Drones

			// Extractor @11
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Extractor);

			// 6 Zerglings @10
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling);
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling);
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling);

			break;

		case Strategy::BuildOrder::TWELVE_HATCH:

			// Drone @4
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @5
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @6
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @7
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @8
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Overlord @9
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Overlord);

			// Drone @9
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @10
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @11
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Drone @12
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Hatchery @12
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone);

			// Spawning Pool @11
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Spawning_Pool);

			// Extractor @11
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Extractor);

			// 6 Zerglings @10
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling);
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling);
			fillBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling);

			break;

		}
	}

	void Strategy::fillBuildOrderItem(UnitType item) {
		buildOrder.emplace(item);
	}
}