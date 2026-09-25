#ifndef OBSERVERS_H
#define OBSERVERS_H
// OWNER: Member A | Observer (ConcreteObservers). Both keep real data, not just print.
#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include "IncidentObserver.h"

class OperatorDashboard : public IncidentObserver {
public:
    void update(const Incident& incident,
                const std::string& oldStatus, const std::string& newStatus) override;
    void printBoard() const;                  // live table: id, type, area, status
    std::size_t activeCount() const;           // incidents not yet Resolved
private:
    std::map<int, std::string> board_;         // id -> formatted row
    std::map<int, bool> resolved_;
};

class IncidentLogger : public IncidentObserver {
public:
    void update(const Incident& incident,
                const std::string& oldStatus, const std::string& newStatus) override;
    void printLog() const;                     // numbered audit trail
    std::size_t entryCount() const { return entries_.size(); }
private:
    std::vector<std::string> entries_;
};
#endif
