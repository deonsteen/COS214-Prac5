#include "Commands.h"
#include <stdexcept>
#include "AccessControlSystem.h"
#include "ExternalAlertService.h"
#include "Incident.h"
#include "ResponseUnit.h"

DispatchUnitCommand::DispatchUnitCommand(ResponseUnit&unit, Incident&incident): unit_(unit), incident_(incident) {

}

void DispatchUnitCommand::execute() {
    incident_.dispatch();
    unit_.dispatchTo(incident_);  
}

void DispatchUnitCommand::undo() {
    unit_.recall(incident_);
}

std::string DispatchUnitCommand::describe() const {
    return "Dispatch " + unit_.name() + " to incident #" + std::to_string(incident_.id());
}

LockAreaCommand::LockAreaCommand(AccessControlSystem&access, const std::string&area,AccessMode mode): access_(access), area_(area), mode_(mode), previous_(AccessMode::Unlocked) {

}

void LockAreaCommand::execute() {
    if (!access_.hasArea(area_))
        throw std::invalid_argument("unknown area '"+area_+"'");
    previous_=access_.modeOf(area_);
    access_.setMode(area_, mode_);
}

void LockAreaCommand::undo() {
    access_.setMode(area_, previous_);
}

std::string LockAreaCommand::describe() const {
    return std::string("Set ")+area_+" to "+toString(mode_);
}

IssueAlertCommand::IssueAlertCommand(ExternalAlertService&alerts, const std::string&area,Severity severity, const std::string&message): alerts_(alerts), area_(area), severity_(severity), message_(message) {

}

void IssueAlertCommand::execute() {
    if (!alerts_.sendAlert(area_, severity_, message_))
        throw std::runtime_error("alert for "+area_+" could not be delivered");
}

void IssueAlertCommand::undo() {
    alerts_.sendAlert(area_, Severity::Low, "CANCELLED: "+message_);
}

std::string IssueAlertCommand::describe() const {
    return std::string(toString(severity_))+" alert to " + area_ + ": \"" + message_ +"\"";
}

EvacuateAreaCommand::EvacuateAreaCommand(AccessControlSystem&access,ExternalAlertService&alerts, const std::string&area): access_(access), alerts_(alerts), area_(area), previous_(AccessMode::Unlocked) {

}

void EvacuateAreaCommand::execute() {
    if (!access_.hasArea(area_))
        throw std::invalid_argument("unknown area '"+area_ + "'");
    previous_ = access_.modeOf(area_);
    access_.setMode(area_, AccessMode::Unlocked);

    if (!alerts_.sendAlert(area_, Severity::Critical,"EVACUATE "+area_+" NOW via the nearest exit")) {
        access_.setMode(area_, previous_);              
        throw std::runtime_error("evacuation alert for "+area_ + " failed");
    }
}

void EvacuateAreaCommand::undo() {
    access_.setMode(area_, previous_);
    alerts_.sendAlert(area_, Severity::Moderate, "Evacuation of "+area_ +" cancelled");
}

std::string EvacuateAreaCommand::describe() const {
    return "Evacuate "+area_;
}
