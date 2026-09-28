#ifndef EMERGENCY_MEDIATOR_H
#define EMERGENCY_MEDIATOR_H
#include <string>
class ResponseUnit;
class Incident;

class EmergencyMediator{
public:
    virtual ~EmergencyMediator() {}
    virtual void registerUnit(ResponseUnit& unit) = 0;
    virtual void unitDispatched(ResponseUnit& from, Incident& incident) = 0;
    virtual void unitRecalled(ResponseUnit& from, Incident& incident) = 0;
      virtual void supportRequested(ResponseUnit& from, Incident& incident, const std::string& reason) = 0;
};
#endif
