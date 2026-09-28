#ifndef EMERGENCY_RESPONSE_FACADE_H
#define EMERGENCY_RESPONSE_FACADE_H
#include "AccessControlSystem.h"
#include "ExternalAlertService.h"
#include "Incident.h"
#include "OperatorConsole.h"
#include "ResponseUnit.h"

class EmergencyResponseFacade{
public:
      EmergencyResponseFacade(OperatorConsole& console, AccessControlSystem& access, ExternalAlertService& alerts, ResponseUnit& security): console_(console), access_(access), alerts_(alerts), security_(security) {}
    bool respondToIncident(Incident& incident, ResponseUnit& leadUnit);
    bool declareLockdown(Incident& incident);
    bool standDown(Incident& incident);
private:
      OperatorConsole& console_;         
    AccessControlSystem& access_;
    ExternalAlertService& alerts_;
    ResponseUnit& security_;
};
#endif
