#ifndef INCIDENT_H
#define INCIDENT_H
// OWNER: Member A | State (Context) + Observer (Subject)
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "Types.h"

class IncidentState;
class IncidentObserver;

class InvalidTransition : public std::logic_error {
public:
    explicit InvalidTransition(const std::string& msg) : std::logic_error(msg) {}
};

class Incident {
public:
    Incident(int id, IncidentType type, Severity severity,
             const std::string& area, const std::string& description);
    ~Incident();                                  // defined in .cpp (unique_ptr to incomplete type)
    Incident(const Incident&) = delete;
    Incident& operator=(const Incident&) = delete;

    int id() const                     { return id_; }
    IncidentType type() const          { return type_; }
    Severity severity() const          { return severity_; }
    const std::string& area() const    { return area_; }
    const std::string& description() const { return description_; }
    std::string status() const;                   // current state's name()

    // Lifecycle events, delegated to the current state. Throw InvalidTransition if illegal.
    void dispatch();
    void startWork();
    void resolve();

    // non-owning. attach() immediately calls observer.update(*this, "", status())
    // so a new observer sees the current status. Observers must outlive the incident.
    void attach(IncidentObserver& observer);
    void detach(IncidentObserver& observer);

    // Called by IncidentState subclasses only. Save old name FIRST, then replace state_
    // (this deletes the calling state), then notifyObservers(old, new).
    void changeState(std::unique_ptr<IncidentState> next);

private:
    void notifyObservers(const std::string& oldStatus, const std::string& newStatus);

    int id_;
    IncidentType type_;
    Severity severity_;
    std::string area_;
    std::string description_;
    std::unique_ptr<IncidentState> state_;        // owning
    std::vector<IncidentObserver*> observers_;    // non-owning
};
#endif
