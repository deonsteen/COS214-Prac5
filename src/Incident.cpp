#include "Incident.h"
#include "IncidentState.h"
#include "IncidentObserver.h"
#include <algorithm>
#include <cstddef>
#include <iostream>

using namespace std;

Incident::Incident(int id, IncidentType type, Severity severity, const string &area, const string &description)
{
    // id_ member initializer, sets member variable to passed in value, id_ private class member
    // state gets initialised as reported
    id_ = id;
    type_ = type;
    severity_ = severity;
    area_ = area;
    description_ = description;
    state_.reset(new ReportedState());
}

Incident::~Incident() {}

string Incident::status() const { return state_->name(); }

// virtual calls at runtime
// *this is passed in so that change state works on the right incident
void Incident::dispatch() { state_->dispatch(*this); }
void Incident::startWork() { state_->startWork(*this); }
void Incident::resolve() { state_->resolve(*this); }

/*
- parameter is a reference to the interface,
    attach accepts anything that inherits from IncidentObserver
- obeservers_ is a list of pointers
- push_back stores the address of the observer 
    so the observer poutlives the incident
-*this is the incident so observer can read its attributes
- "" has the old status in its empty because 
    its the first time the observer has seen the incident
- status() is the current states name
*/
void Incident::attach(IncidentObserver &observer)
{
    observers_.push_back(&observer);
    observer.update(*this, "", status());
}

/*
- searches the vector for any element = observer
    shifts the elements still needed forward
    and tells where the vector ends 
- deletes the leftover tail so the vector shrinks 
*/
void Incident::detach(IncidentObserver &observer)
{
    observers_.erase(std::remove(observers_.begin(), observers_.end(), &observer), observers_.end());
}

/*
where the patterns meet to change the state and update the observers
- Parameter taken by value changing ownership 
*/
void Incident::changeState(std::unique_ptr<IncidentState> next)
{
    string oldState = state_->name();
    state_ = move(next);
    string newState = state_->name();
    cout << "[State] Incident #" << id_ << " (" << toString(type_) << ", " << area_ << "): " << oldState << " -> " << newState << endl;
    notifyObservers(oldState, newState);
}

void Incident::notifyObservers(const std::string &oldStatus, const std::string &newStatus)
{
    for (std::size_t i = 0; i < observers_.size(); ++i)
    {
        observers_[i]->update(*this, oldStatus, newStatus);
    }
}
