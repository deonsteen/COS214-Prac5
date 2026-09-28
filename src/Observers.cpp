#include "Observers.h"
#include "Incident.h"
#include <iostream>
#include <sstream>

using namespace std;

// ---------------- OperatorDashboard ----------------

void OperatorDashboard::update(const Incident &incident, const std::string &, const std::string &newStatus)
{
    ostringstream oss;
    oss << "#" << incident.id() << " " << toString(incident.type()) << " @ " << incident.area() << " [" << newStatus << "]";
    board_[incident.id()] = oss.str();
    resolved_[incident.id()] = (newStatus == "Resolved");
}

void OperatorDashboard::printBoard() const
{
    cout << "[Dashboard] ---- operator board ----" << endl;
    for (map<int, string>::const_iterator it = board_.begin(); it != board_.end(); ++it)
    {
        cout << "[Dashboard] " << it->second << endl;
    }
}

std::size_t OperatorDashboard::activeCount() const
{
    size_t count = 0;
    for (map<int, bool>::const_iterator it = resolved_.begin(); it != resolved_.end(); ++it)
    {
        if (!it->second)
        {
            ++count;
        }
    }
    return count;
}

// ---------------- IncidentLogger ----------------

void IncidentLogger::update(const Incident &incident, const std::string &oldStatus, const std::string &newStatus)
{
    ostringstream oss;
    oss << "Incident #" << incident.id() << " (" << toString(incident.type()) << ", " << incident.area() << "): ";
    if (oldStatus.empty())
    {
        oss << "opened as " << newStatus;
    }
    else
    {
        oss << oldStatus << " -> " << newStatus;
    }
    entries_.push_back(oss.str());
}

void IncidentLogger::printLog() const
{
    cout << "[Logger] ---- incident log ----" << endl;
    for (size_t i = 0; i < entries_.size(); ++i)
    {
        cout << "[Logger] " << (i + 1) << ". " << entries_[i] << endl;
    }
}
