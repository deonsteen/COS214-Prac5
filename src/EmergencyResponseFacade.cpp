#include "EmergencyResponseFacade.h"
#include <iostream>
#include <memory>
#include "Commands.h"

namespace {
bool abort(const std::string& operation, const std::string& why) {
      std::cout << "[Facade] " << operation << " ABORTED: " << why << "\n";
    return false;
}
}

bool EmergencyResponseFacade::respondToIncident(Incident& incident, ResponseUnit& leadUnit) {
    const std::string op = "respondToIncident";
    std::cout << "[Facade] " << op << " #" << incident.id() << " (lead: " << leadUnit.name() << ")\n";

    std::cout << "[Facade] step 1/3: dispatch lead unit\n";
    if(!console_.submit(std::unique_ptr<Command>(new DispatchUnitCommand(leadUnit, incident))))
         return abort(op, "dispatch not approved");

    std::cout << "[Facade] step 2/3: public alert\n";
    if(!console_.submit(std::unique_ptr<Command>(new IssueAlertCommand(
            alerts_, incident.area(), incident.severity(), incident.description()))))
        return abort(op, "public alert failed");

    std::cout << "[Facade] step 3/3: work starts on scene\n";
    try{
        incident.startWork();
    }catch (const InvalidTransition& e) {
        return abort(op, e.what());
       }
    std::cout << "[Facade] " << op << " complete\n";
    return true;
}

bool EmergencyResponseFacade::declareLockdown(Incident& incident) {
    const std::string op = "declareLockdown";
    std::cout << "[Facade] " << op << " #" << incident.id() << " (" << incident.area() << ")\n";

    std::cout << "[Facade] step 1/3: dispatch security\n";
    if(!console_.submit(std::unique_ptr<Command>(new DispatchUnitCommand(security_, incident))))
         return abort(op, "security dispatch rejected");

    std::cout << "[Facade] step 2/3: lock " << incident.area() << "\n";
    if(!console_.submit(std::unique_ptr<Command>(
         new LockAreaCommand(access_, incident.area(), AccessMode::Locked))))
           return abort(op, "area could not be locked unfortunately");

    std::cout << "[Facade] step 3/3: lockdown alert\n";
    if (!console_.submit(std::unique_ptr<Command>(new IssueAlertCommand(
            alerts_, incident.area(), Severity::Critical,
            "LOCKDOWN in " + incident.area() + ": stay inside, doors are locked"))))
        return abort(op, "lockdown alert failed");

    std::cout << "[Facade] " << op << " complete\n";
    return true;
}

bool EmergencyResponseFacade::standDown(Incident& incident) {
    const std::string op = "standDown";
    std::cout << "[Facade] " << op << " #" << incident.id() << " (" << incident.area() << ")\n";

    std::cout << "[Facade] step 1/3: reopen " << incident.area() << "\n";
    if (!console_.submit(std::unique_ptr<Command>(
            new LockAreaCommand(access_, incident.area(), AccessMode::Unlocked))))
        return abort(op, "area could not be reopened");

    std::cout << "[Facade] step 2/3: resolve incident\n";
    try{
        incident.resolve();
    }catch (const InvalidTransition& e){
          return abort(op, e.what());
    }

    std::cout << "[Facade] step 3/3: all-clear alert\n";
    if(!console_.submit(std::unique_ptr<Command>(new IssueAlertCommand( alerts_, incident.area(), Severity::Low, "ALL CLEAR: " + incident.area() + " reopened"))))

        return abort(op, "all-clear alert failed");
    std::cout << "[Facade] " << op << " complete\n";
    return true;
}
