#ifndef EMERGENCY_COORDINATOR_H
#define EMERGENCY_COORDINATOR_H
#include <vector>
#include "EmergencyMediator.h"

class EmergencyCoordinator : public EmergencyMediator {
public:
      void registerUnit(ResponseUnit& unit) override;
    void unitDispatched(ResponseUnit& from, Incident& incident) override;
    void unitRecalled(ResponseUnit& from, Incident& incident) override;
    void supportRequested(ResponseUnit& from, Incident& incident, const std::string& reason) override;
private:
    std::vector<ResponseUnit*> units_;             
};
#endif
