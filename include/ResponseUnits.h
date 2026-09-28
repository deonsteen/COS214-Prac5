#ifndef RESPONSE_UNITS_H
#define RESPONSE_UNITS_H
#include <string>
#include "ResponseUnit.h"

class AccessControlSystem;
class ExternalAlertService;

class SecurityTeam : public ResponseUnit {    
public:
      explicit SecurityTeam(EmergencyMediator& m) : ResponseUnit("Security", m) {}
    bool respondsTo(const Incident& incident) const override;
    void dispatchTo(Incident& incident) override;
    void recall(Incident& incident) override;
    void assist(Incident& incident, const std::string& reason) override;
};

class MedicalResponder : public ResponseUnit {  
public:
     explicit MedicalResponder(EmergencyMediator& m) : ResponseUnit("Medical", m) {}
    bool respondsTo(const Incident& incident) const override;
    void dispatchTo(Incident& incident) override;
    void recall(Incident& incident) override;
    void assist(Incident& incident, const std::string& reason) override;
};

class FacilitiesCrew : public ResponseUnit {     
public:
    FacilitiesCrew(EmergencyMediator& m, AccessControlSystem& access)
        : ResponseUnit("Facilities", m), access_(access) {}
    bool respondsTo(const Incident& incident) const override;
    void dispatchTo(Incident& incident) override;
    void recall(Incident& incident) override;
    void assist(Incident& incident, const std::string& reason) override;
private:
    AccessControlSystem& access_;               
};

class CommsTeam : public ResponseUnit {            
public:
    CommsTeam(EmergencyMediator& m, ExternalAlertService& alerts) : ResponseUnit("Comms", m), alerts_(alerts) {}
      bool respondsTo(const Incident& incident) const override;
    void dispatchTo(Incident& incident) override;
    void recall(Incident& incident) override;
    void assist(Incident& incident, const std::string& reason) override;
private:
    ExternalAlertService& alerts_;            
};
#endif
