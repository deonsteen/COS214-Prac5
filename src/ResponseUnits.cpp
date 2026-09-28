#include "ResponseUnits.h"
#include <iostream>
#include "AccessControlSystem.h"
#include "ExternalAlertService.h"

bool SecurityTeam::respondsTo(const Incident& incident) const{
    return incident.type() == IncidentType::Intrusion || incident.type() == IncidentType::Fire;
}
void SecurityTeam::dispatchTo(Incident& incident){
    std::cout << "[Security] team is on the way to " << incident.area() << " (incident #" << incident.id() << ")\n";
               announceDispatch(incident);
}
void SecurityTeam::recall(Incident& incident){
    std::cout << "[Security] team recalled from " << incident.area() << "\n";
       announceRecall(incident);
}
void SecurityTeam::assist(Incident& incident, const std::string& reason){
    std::cout << "[Security] crowd control and cordon at " << incident.area() << " (" << reason << ")\n";
        }

bool MedicalResponder::respondsTo(const Incident& incident) const{
       return incident.type() == IncidentType::Medical || incident.type() == IncidentType::Fire || incident.type() == IncidentType::Hazmat;
}
void MedicalResponder::dispatchTo(Incident& incident){
      std::cout << "[Medical] paramedics en route to " << incident.area() << " (incident #" << incident.id() << ")\n";
        announceDispatch(incident);
}
void MedicalResponder::recall(Incident& incident){
        std::cout << "[Medical] paramedics recalled from " << incident.area() << "\n";
       announceRecall(incident);
}
void MedicalResponder::assist(Incident& incident, const std::string& reason){
      std::cout << "[Medical] ambulance staged outside " << incident.area() << " (" << reason << ")\n";
}

bool FacilitiesCrew::respondsTo(const Incident& incident) const{
    return incident.type() == IncidentType::Fire || incident.type() == IncidentType::Hazmat;
}
void FacilitiesCrew::dispatchTo(Incident& incident) {
    std::cout << "[Facilities] crew en route to " << incident.area() << " (incident #" << incident.id() << ")\n";
           announceDispatch(incident);
}
void FacilitiesCrew::recall(Incident& incident){
     std::cout << "[Facilities] crew recalled from " << incident.area() << "\n";
          announceRecall(incident);
}
void FacilitiesCrew::assist(Incident& incident, const std::string& reason) {
    std::cout << "[Facilities] restricting " << incident.area() << " to responder access only (" << reason << ")\n";
    if(!access_.setMode(incident.area(), AccessMode::Restricted))
            std::cout << "[Facilities] could not change access for " << incident.area() << "\n";
          }

bool CommsTeam::respondsTo(const Incident& incident) const{
    return incident.severity() != Severity::Low;
}

void CommsTeam::dispatchTo(Incident& incident){
        std::cout << "[Comms] officer assigned to incident #" << incident.id() << "\n";
    announceDispatch(incident);
}
void CommsTeam::recall(Incident& incident) {
        std::cout << "[Comms] officer stood down from incident #" << incident.id() << "\n";
       announceRecall(incident);
     }     
void CommsTeam::assist(Incident& incident, const std::string& reason) {
    std::cout << "[Comms] PA announcement for " << incident.area() << " (" << reason << ")\n";
    std::string text = std::string("Emergency services attending ") + toString(incident.type()) + " at " + incident.area();

    if(!alerts_.sendAlert(incident.area(), incident.severity(), text))
         std::cout << "[Comms] PA announcement FAILED, falling back to radio\n";
}
