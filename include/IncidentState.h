#ifndef INCIDENT_STATE_H
#define INCIDENT_STATE_H
// OWNER: Member A | State (State + ConcreteStates)
// Reported --dispatch--> Dispatched --startWork--> InProgress --resolve--> Resolved
// dispatch() is also legal in Dispatched/InProgress (extra units; state unchanged).
// Every other event throws InvalidTransition. Resolved rejects everything.
// RULE: incident.changeState(...) must be the LAST statement (it deletes `this`).
#include <string>
#include "Incident.h"

class IncidentState {
public:
    virtual ~IncidentState() {}
    virtual std::string name() const = 0;
    virtual void dispatch(Incident& incident);     // defaults throw InvalidTransition
    virtual void startWork(Incident& incident);
    virtual void resolve(Incident& incident);
};

class ReportedState : public IncidentState {
public:
    std::string name() const override;
    void dispatch(Incident& incident) override;
};
class DispatchedState : public IncidentState {
public:
    std::string name() const override;
    void dispatch(Incident& incident) override;
    void startWork(Incident& incident) override;
};
class InProgressState : public IncidentState {
public:
    std::string name() const override;
    void dispatch(Incident& incident) override;
    void resolve(Incident& incident) override;
};
class ResolvedState : public IncidentState {
public:
    std::string name() const override;
};
#endif
