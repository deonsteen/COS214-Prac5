#ifndef RESPONSE_UNIT_H
#define RESPONSE_UNIT_H
#include <string>
#include "EmergencyMediator.h"
#include "Incident.h"

class ResponseUnit {
public:
    ResponseUnit(const std::string& name, EmergencyMediator& mediator)
        : name_(name), mediator_(&mediator) {}
    virtual ~ResponseUnit() {}
    const std::string& name() const { return name_; }

    virtual bool respondsTo(const Incident& incident) const = 0;  
    virtual void dispatchTo(Incident& incident) = 0; 
    virtual void recall(Incident& incident) = 0;      
    virtual void assist(Incident& incident, const std::string& reason) = 0;
    void requestSupport(Incident& incident, const std::string& reason){
        mediator_->supportRequested(*this, incident, reason);
      }
    protected:
    void announceDispatch(Incident& incident){ mediator_->unitDispatched(*this, incident); }
    void announceRecall(Incident& incident){ mediator_->unitRecalled(*this, incident); }
    private:
    std::string name_;
    EmergencyMediator* mediator_;          
};
#endif
