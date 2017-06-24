#include "Strategy.h"
#include "BlackCrow.h"
#include <BWEM/bwem.h>
#include "Area.h"
#include "Planned.h"
#include "Macro.h"
#include "Bricks.h"
#include "Common.h"


namespace BlackCrow {

	using namespace BWAPI;
	using namespace Filter;

	Strategy::Strategy(BlackCrow &parent) : bc(parent), start("START") {
		unitMix = std::make_unique<UnitMix>(bc);
	}

	void Strategy::onStart() {
		unitMix->set(BWAPI::UnitTypes::Zerg_Drone, 1, true);

		// Initial Scout
		BrickPtr initialScout = Bricks::makeBlank("Initial Scout");
		initialScout->requiredOnce([&bc = bc]() { return bc.macro.getUsedSupply() >= 18; });
		initialScout->once([&bc = bc]() { bc.army.startInitialScout(); });
		start.successor(initialScout);
		
		// Being rushed
		BrickPtr rushDefense = Bricks::makeBlank("Rush Defense");
		rushDefense->requiredOnce([&bc = bc]() { return bc.enemy.isRushing(); });

		// Buildorders against enemy Race
		// Protoss
		BrickPtr protossBo = Bricks::makeBlank("Enemy Protoss BO");
		protossBo->requiredOnce([]() { return Broodwar->enemy()->getRace().getName() == "Protoss"; });
		start.successor(protossBo);
		BrickPtr protossBoLast = buildorderOverpool(protossBo);

		// Terran
		BrickPtr terranBo = Bricks::makeBlank("Enemy Terran BO");
		terranBo->requiredOnce([]() { return Broodwar->enemy()->getRace().getName() == "Terran"; });
		start.successor(terranBo);
		BrickPtr terranBoLast = buildorderTwelveHatch(terranBo);

		// Zerg
		BrickPtr zergBo = Bricks::makeBlank("Enemy Zerg BO");
		zergBo->requiredOnce([]() { return Broodwar->enemy()->getRace().getName() == "Zerg"; });
		start.successor(zergBo);
		BrickPtr zergBoLast = buildorderNinePool(zergBo);

		// Random
		BrickPtr randomBo = Bricks::makeBlank("Random BO");
		randomBo->requiredOnce([]() { return Broodwar->enemy()->getRace().getName() == "Unknown"; });
		start.successor(randomBo);
		BrickPtr randomBoLast = buildorderOverpool(randomBo);

		// Disable race specific openings when enemy plays random
		protossBo->disableSelfWhenActive(randomBo);
		terranBo->disableSelfWhenActive(randomBo);
		zergBo->disableSelfWhenActive(randomBo);

		// Go dynamic
		BrickPtr dynamicStart = Bricks::makeBlank("Dynamic decisions");
		dynamicStart->once([]() { Broodwar->sendText("Dynamic stuff starts here!"); });

		protossBoLast->successor(dynamicStart);
		terranBoLast->successor(dynamicStart);
		zergBoLast->successor(dynamicStart);
		randomBoLast->successor(dynamicStart);


	}

	BrickPtr Strategy::buildorderOverpool(BrickPtr predecessor) {
		Bricks::SucessorInPredecessorChain chain(predecessor);
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @4", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @5", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @6", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @7", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @8", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Overlord @9", UnitTypes::Zerg_Overlord, bc.macro.startPosition));
		chain.set(Bricks::makePlanBuildingOnce	(bc, "Pool @9", UnitTypes::Zerg_Spawning_Pool, TilePosition(bc.macro.startPosition), false));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @8", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @9", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @10", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanExtractorOnce	(bc, "Extractor @11")); // Go parallel for case extractor blocked?
		chain.set(Bricks::makePlanUnitOnce		(bc, "Zergling @10", UnitTypes::Zerg_Zergling, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Zergling @10", UnitTypes::Zerg_Zergling, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Zergling @10", UnitTypes::Zerg_Zergling, bc.macro.startPosition));
		return chain.getLastSuccessor();
	}

	BrickPtr Strategy::buildorderTwelveHatch(BrickPtr predecessor) {
		Bricks::SucessorInPredecessorChain chain(predecessor);
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @4", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @5", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @6", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @7", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @8", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Overlord @9", UnitTypes::Zerg_Overlord, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @9", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @10", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @11", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @12", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanBuildingOnce	(bc, "Hatchery @12", UnitTypes::Zerg_Hatchery, TilePosition(bc.macro.startPosition), false));
		chain.set(Bricks::makePlanBuildingOnce	(bc, "Pool @11", UnitTypes::Zerg_Spawning_Pool, TilePosition(bc.macro.startPosition), false));
		chain.set(Bricks::makePlanExtractorOnce	(bc, "Extractor @10")); // Go parallel for case extractor blocked?
		chain.set(Bricks::makePlanUnitOnce		(bc, "Zergling @10", UnitTypes::Zerg_Zergling, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Zergling @10", UnitTypes::Zerg_Zergling, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Zergling @10", UnitTypes::Zerg_Zergling, bc.macro.startPosition));
		return chain.getLastSuccessor();
	}

	BrickPtr Strategy::buildorderNinePool(BrickPtr predecessor) {
		Bricks::SucessorInPredecessorChain chain(predecessor);
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @4", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @5", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @6", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @7", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @8", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanBuildingOnce	(bc, "Pool @9", UnitTypes::Zerg_Spawning_Pool, TilePosition(bc.macro.startPosition), false));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @8", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanExtractorOnce	(bc, "Extractor @9")); // Go parallel for case extractor blocked?
		chain.set(Bricks::makePlanUnitOnce		(bc, "Overlord @8", UnitTypes::Zerg_Overlord, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Drone @8", UnitTypes::Zerg_Drone, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Zergling @10", UnitTypes::Zerg_Zergling, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Zergling @10", UnitTypes::Zerg_Zergling, bc.macro.startPosition));
		chain.set(Bricks::makePlanUnitOnce		(bc, "Zergling @10", UnitTypes::Zerg_Zergling, bc.macro.startPosition));
		return chain.getLastSuccessor();
	}

	void Strategy::onFrame() {
		start.run();
	}


	/*
	void Strategy::dynamicDecision() {
		
		// Check if we need drones, add them to the mix
		if (bc.macro.getWorkersNeededForSaturation() - bc.macro.getCurrentlyPlannedAmount(UnitTypes::Zerg_Drone) > 0 || bc.macro.getTotalWorkerAmount() >= bc.config.maxDrones) {
			if (!unitMix->exists(UnitTypes::Zerg_Drone))
				unitMix->set(UnitTypes::Zerg_Drone, 1, true);
		} else {
			if (unitMix->exists(UnitTypes::Zerg_Drone))
				unitMix->remove(UnitTypes::Zerg_Drone);
		}

		// Collect up to 100 gas
		if (Broodwar->self()->gas() + Broodwar->self()->spentGas() < 100) {
			if (bc.macro.getGasWorkerSlotsAvailable() > 0)
				if (bc.macro.getTotalGasWorkerAmount() < 3)
					bc.macro.addGasWorker();
		} else {
			if (bc.macro.getTotalGasWorkerAmount() > 0)
				bc.macro.removeGasWorker();
		}

		// Check if we have a spawning pool
		auto ownUnits = Broodwar->self()->getUnits();
		if (std::find_if(ownUnits.begin(), ownUnits.end(), [](Unit unit) { return unit->getType() == UnitTypes::Zerg_Spawning_Pool; }) != ownUnits.end() && !bc.macro.getCurrentlyPlannedAmount(BWAPI::UnitTypes::Zerg_Spawning_Pool)) {

			// Zergling Speed
			if (bc.macro.getUnreservedResources().gas >= 100 && !bc.macro.isCurrentlyPlanned(UpgradeTypes::Metabolic_Boost) && Broodwar->self()->getUpgradeLevel(UpgradeTypes::Metabolic_Boost) <= 0) {
				bc.macro.planUpgrade(UpgradeTypes::Metabolic_Boost, 1);
			}

			// Building Zerglings
			if (!unitMix->exists(UnitTypes::Zerg_Zergling))
				unitMix->set(UnitTypes::Zerg_Zergling, 2, true);
		}
		
		if (unitMix->size() > 0) {

			productionMultiplierMinerals = bc.macro.getAverageMineralsPerFrame() / unitMix->mineralPerFrame();
			//productionMultiplierGas = bc.macro.getAverageGasPerFrame() / unitMix->gasPerFrame(); // TODO Gas units not included
			productionMultiplierLarvae = bc.macro.getAverageLarvaePerFrame() / unitMix->larvaPerFrame();
			productionMultiplier = std::min(productionMultiplierMinerals, productionMultiplierLarvae);

			// Supply first
			if (bc.macro.getFreeSupply() / (unitMix->supplyPerFrame() * productionMultiplier) < UnitTypes::Zerg_Overlord.buildTime() && bc.macro.getFreeSupply() <= 20) {
				bc.macro.planUnit(UnitTypes::Zerg_Overlord, bc.macro.startPosition);
			}

			// Drone
			if (unitMix->peek() == UnitTypes::Zerg_Drone) {
				if (bc.macro.getUnreservedResources().minerals >= 50 && bc.macro.getUnreservedLarvaeAmount() > 0) {
					bc.macro.buildWorkerDrone();
					unitMix->set(UnitTypes::Zerg_Drone, std::max(unitMix->get(UnitTypes::Zerg_Drone) - 0.1, 0.15), false);
					unitMix->pop();
				}
					
			// Combat  Units
			} else {
				if (bc.macro.getUnreservedResources().minerals >= unitMix->peek().mineralPrice() && bc.macro.getUnreservedLarvaeAmount() > 0)
					bc.macro.planUnit(unitMix->pop(), bc.macro.startPosition);
			}

			// Additional Hatcheries
			if (productionMultiplierLarvae < productionMultiplierMinerals && bc.macro.getTotalLarvaeAmount() <= 0) {
				int amountPlannedHatcheries = bc.macro.getCurrentlyPlannedAmount(UnitTypes::Zerg_Hatchery);

				if (amountPlannedHatcheries <= 0) {
					if (bc.macro.getUnreservedResources().minerals >= 250)
						bc.macro.planBuilding(UnitTypes::Zerg_Hatchery, bc.builder.getBuildingSpot(UnitTypes::Zerg_Hatchery));
				} else {
					if ((double)bc.macro.getUnreservedResources().minerals / ((double)amountPlannedHatcheries * (double)300) >= 0.75)
						bc.macro.planBuilding(UnitTypes::Zerg_Hatchery, bc.builder.getBuildingSpot(UnitTypes::Zerg_Hatchery));
				}
			}
		}

		// Give up
		if (Broodwar->self()->minerals() < 50 && Broodwar->self()->supplyUsed() <= 0) {
			Broodwar->sendText("gaw gaw!");
			Broodwar->leaveGame();
		}
	}
	*/
}