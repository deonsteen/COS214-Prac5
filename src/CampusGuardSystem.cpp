#include "CampusGuardSystem.h"
#include <iostream>

namespace{
struct AS {
    const char* area;
    int zoneCode;
};
const AS kAreas[] ={
    {"CHEM", 101},   
    {"LIB", 102},   
};
}  


 
CampusGuardSystem::CampusGuardSystem() : 
    legacyPA_(),
    alertService_(legacyPA_),
    accessControl_(),
    dashboard_(),
    logger_(),
    coordinator_(),
    units_(),
    security_(nullptr),
    medical_(nullptr),
    facilities_(nullptr),
    comms_(nullptr),
    incidents_(),
    console_(),
    facade_(),
    nextIncidentId_(1){
                    std::cout << "------CampusGuard starting up--------\n";
 
    for(std::size_t i = 0; i < sizeof(kAreas) / sizeof(kAreas[0]); ++i){
    accessControl_.addArea(kAreas[i].area);
      legacyPA_.PA_RegisterZone(kAreas[i].zoneCode);
        alertService_.mapArea(kAreas[i].area, kAreas[i].zoneCode);
    }
 
    // These are the response units
    security_= new SecurityTeam(coordinator_);
    units_.push_back(std::unique_ptr<ResponseUnit>(security_));

    medical_ = new MedicalResponder(coordinator_);
    units_.push_back(std::unique_ptr<ResponseUnit>(medical_));

    facilities_ = new FacilitiesCrew(coordinator_, accessControl_);
    units_.push_back(std::unique_ptr<ResponseUnit>(facilities_));

    comms_= new CommsTeam(coordinator_, alertService_);
    units_.push_back(std::unique_ptr<ResponseUnit>(comms_));
 
    for(std::size_t i = 0; i < units_.size(); ++i) {
     coordinator_.registerUnit(*units_[i]);
    }
 
    // Facade pattern
    facade_.reset(new EmergencyResponseFacade(console_, accessControl_, alertService_, *security_));
                       std::cout << " ------CampusGuard ready------\n";
}
 
Incident& CampusGuardSystem::openIncident(IncidentType type, Severity severity, const std::string& area, const std::string& description){
    incidents_.push_back(std::unique_ptr<Incident>(
          new Incident(nextIncidentId_++, type, severity, area, description)));
    Incident& incident = *incidents_.back();
 
    std::cout << "[System] Opened incident #" << incident.id() << ": " << toString(type) << ", " << toString(severity) << ", area " << area << " - " << description << "\n";
 
    incident.attach(dashboard_);
    incident.attach(logger_);
    return incident;
    //remeber observers outlive their incidents
}