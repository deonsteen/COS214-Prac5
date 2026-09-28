#include <iostream>
#include "CampusGuardSystem.h"
#include "Commands.h"

void CampusGuardSystem::runScenarioOne() {
    std::cout << "#  SCENARIO 1: Chemistry lab fire! \n";
 
    // step 1. fire reported
    std::cout << "\n Step 1: fire reported in the chemistry building\n";
    Incident& fire = openIncident(IncidentType::Fire, Severity::Critical, "CHEM", "solvent fire in organic chemistry lab 2");
 
    // step 2: Facade responds
    std::cout << "\n Step 2: operator responds \n";
    if(!facade_->respondToIncident(fire, *medical_)) { std::cout << "[System] respondToIncident did not complete\n"; }
 
    std::cout << "\n Step 3: operator orders an evacuation directly\n";
    console_.submit(std::unique_ptr<Command>(
             new EvacuateAreaCommand(accessControl_, alertService_, "CHEM")));
 
    // 4. Fire is out: unlock, resolve, all clear.
    std::cout << "\n Step 4: fire is extinguished, operator stands down\n";
    if(!facade_->standDown(fire)) {
        std::cout << "[System] standDown operation did not complete\n";
    }
    std::cout << "\n Step 5: Observer records\n";
    dashboard_.printBoard();
    logger_.printLog();
}
 