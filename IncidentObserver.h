#ifndef INCIDENT_OBSERVER_H
#define INCIDENT_OBSERVER_H
// OWNER: Member A | Observer (Observer interface)
#include <string>
class Incident;

class IncidentObserver {
public:
    virtual ~IncidentObserver() {}
    virtual void update(const Incident& incident,
                         const std::string& oldStatus, const std::string& newStatus) = 0;
};
#endif
