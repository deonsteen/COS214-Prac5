#include "EmergencyCoordinator.h"
#include <iostream>
#include <string>
#include "Incident.h"
#include "ResponseUnit.h"
#include "Types.h"

namespace {
 
std::string describe(const Incident& incident) {
    return "#" + std::to_string(incident.id()) + " (" + toString(incident.type()) + ", " + toString(incident.severity()) + ", area " + incident.area() + ")";
}
 
// here we ask every responding unit to assist and record how many actually assisted
int AskForAssistance(const std::vector<ResponseUnit*>& units, ResponseUnit& from, Incident& incident, const std::string& reason){
    int asked = 0;
    for (std::size_t i = 0; i < units.size(); ++i) {
        ResponseUnit* unit = units[i];
        if (unit == &from || !unit->respondsTo(incident)) {
            continue;}

        std::cout << "[Coordinator] is asking " << unit->name() << " to assist\n";
        unit->assist(incident, reason);
        ++asked;
    }
    if(asked == 0){
        std::cout << "[Coordinator]  says -> no other unit is needed for this incident\n";
    }
    return asked;
}
}  
 
void EmergencyCoordinator::registerUnit(ResponseUnit& unit) {
    for(std::size_t i = 0; i < units_.size(); ++i) {
        if(units_[i] == &unit) {
            std::cout << unit.name() << " who is a coordinator is already registered\n";
return;
        }
    }
      units_.push_back(&unit);
      std::cout << "[Coordinator] Registered " << unit.name() << " unit\n";
}
 
void EmergencyCoordinator::unitDispatched(ResponseUnit& from, Incident& incident) {
    std::cout << "[Coordinator] " << from.name() << " dispatched to " << describe(incident) << " - checking which other units are needed\n";
    AskForAssistance(units_, from, incident, from.name() + " is on scene");
}
 
void EmergencyCoordinator::unitRecalled(ResponseUnit& from, Incident& incident) {
      std::cout << "[Coordinator] " << from.name() << " recalled from " << describe(incident) << " - other units keep their current tasks\n";
}
 
void EmergencyCoordinator::supportRequested(ResponseUnit& from, Incident& incident, const std::string& reason){
     std::cout << "[Coordinator] " << from.name() << " requested support at " << describe(incident) << ": " << reason << "\n";
    AskForAssistance(units_, from, incident, reason);
}
 