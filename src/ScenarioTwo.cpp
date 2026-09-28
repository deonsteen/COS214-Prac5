#include <iostream>
#include "CampusGuardSystem.h"
#include "Commands.h"

namespace {
void banner(const std::string& text) {
    std::cout << "\n==================== " << text << " ====================\n";
}

void step(const std::string& text) {
    std::cout << "\n--- " << text << " ---\n";
}
}

void CampusGuardSystem::runScenarioTwo() {
    banner("SCENARIO 2: Library intrusion at night (no facade)");

    step("1. Operator reports the incident");
    Incident& intrusion = openIncident(IncidentType::Intrusion, Severity::Moderate, "LIB","Motion sensors triggered in the Library after hours");

    step("2. Dispatch Security (Command -> State -> Mediator -> Comms -> Adapter)");
    console_.submit(std::unique_ptr<Command>(new DispatchUnitCommand(*security_, intrusion)));

    step("3. Lock the Library");
    console_.submit(std::unique_ptr<Command>(
        new LockAreaCommand(accessControl_, "LIB", AccessMode::Locked)));

    step("4. FAILURE: Library PA amplifier goes offline, alert cannot be delivered");

    legacyPA_.PA_SetAmplifier(alertService_.zoneFor("LIB"), 0);

    console_.submit(std::unique_ptr<Command>(new IssueAlertCommand(
        alertService_, "LIB", Severity::Moderate,
        "Library closed: security operation in progress")));

    step("5. FAILURE: operator tries to resolve before any work has started");
    try {
        intrusion.resolve();
    } 
    catch (const InvalidTransition& e) {
        std::cout << "[Scenario] blocked: " << e.what() << "\n";
    }

    step("6. FAILURE: operator mistypes the area code");
    console_.submit(std::unique_ptr<Command>(
        new LockAreaCommand(accessControl_, "XYZ", AccessMode::Locked)));

    step("7. False alarm on the lock: cancel the previous instruction");
    console_.cancelLast();

    step("8. Technician restores the amplifier; Security works and resolves directly");
    legacyPA_.PA_SetAmplifier(alertService_.zoneFor("LIB"), 1);
    intrusion.startWork();
    intrusion.resolve();

    step("9. FAILURE: dispatching to a resolved incident");
    console_.submit(std::unique_ptr<Command>(new DispatchUnitCommand(*security_, intrusion)));

    step("10. Final board");
    dashboard_.printBoard();
}
