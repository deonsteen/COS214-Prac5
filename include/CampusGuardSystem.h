#ifndef CAMPUS_GUARD_SYSTEM_H
#define CAMPUS_GUARD_SYSTEM_H
#include <memory>
#include <string>
#include <vector>
#include "AccessControlSystem.h"
#include "EmergencyCoordinator.h"
#include "EmergencyResponseFacade.h"
#include "Incident.h"
#include "LegacyCampusPASystem.h"
#include "LegacyPAAdapter.h"
#include "Observers.h"
#include "OperatorConsole.h"
#include "ResponseUnits.h"

class CampusGuardSystem {
public:
    CampusGuardSystem();       
    void runScenarioOne();    
    void runScenarioTwo();     
private:
    Incident& openIncident(IncidentType type, Severity severity, const std::string& area, const std::string& description);
    LegacyCampusPASystem legacyPA_;
    LegacyPAAdapter alertService_;
    AccessControlSystem accessControl_;
    OperatorDashboard dashboard_;
    IncidentLogger logger_;
    EmergencyCoordinator coordinator_;
    std::vector<std::unique_ptr<ResponseUnit>> units_;
    SecurityTeam* security_;          // non-owning shortcuts into units_
    MedicalResponder* medical_;
    FacilitiesCrew* facilities_;
    CommsTeam* comms_;
    std::vector<std::unique_ptr<Incident>> incidents_;
    OperatorConsole console_;
    std::unique_ptr<EmergencyResponseFacade> facade_;   // built LAST in ctor body (needs *security_)
    int nextIncidentId_;
};
#endif
