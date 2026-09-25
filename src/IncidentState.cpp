#include "IncidentState.h"
#include <sstream>

using namespace std;
void IncidentState::dispatch(Incident &incident)
{
    ostringstream oss;
    oss << "Incident #" << incident.id() << " cannot be dispatched from state: " << name();
    string msg = oss.str();
    throw InvalidTransition(msg);
}
void IncidentState::startWork(Incident &incident)
{
    ostringstream oss;
    oss << "Incident #" << incident.id() << " cannot start work from state: " << name();
    string msg = oss.str();
    throw InvalidTransition(msg);
}
void IncidentState::resolve(Incident &incident)
{
    ostringstream oss;
    oss << "Incident #" << incident.id() << " cannot be resolved from state: " << name();
    string msg = oss.str();
    throw InvalidTransition(msg);
}

std::string ReportedState::name() const
{
    return "Reported";
}
void ReportedState::dispatch(Incident &incident)
{
    // dispatched state makes a new state on heap
    // unique_ptr<IncidentState> wraps the pointer to the base class IncidentState
    // changeState only accepts a unique_ptr<IncidentState>
    // Dispatched counts as it inherits from IncidentState
    // incident.changeState(...) changes ownership to the incident
    // changeState swaps the incidents current state, and destroys the old one
    incident.changeState(unique_ptr<IncidentState>(new DispatchedState()));
}

string DispatchedState::name() const
{
    return "Dispatched";
}
void DispatchedState::dispatch(Incident&)
{
    // do nothing state
}
void DispatchedState::startWork(Incident &incident)
{
    incident.changeState(unique_ptr<IncidentState>(new InProgressState()));
}

string InProgressState::name() const
{
    return "InProgress";
}
void InProgressState::dispatch(Incident&)
{
    // do nothing state
}
void InProgressState::resolve(Incident &incident)
{
    incident.changeState(unique_ptr<IncidentState>(new ResolvedState()));
}

string ResolvedState::name() const
{
    return "Resolved";
    // no other overrides needed: dispatch/startWork/resolve all fall through
    // to the base class throw, matching "Resolved rejects everything"
}
