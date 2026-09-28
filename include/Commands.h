#ifndef COMMANDS_H
#define COMMANDS_H 
#include <string>
#include "Command.h"
#include "Types.h"

class Incident;
class ResponseUnit;
class AccessControlSystem;
class ExternalAlertService;

class DispatchUnitCommand : public Command {
public:
    DispatchUnitCommand(ResponseUnit&unit, Incident&incident);
    void execute() override;
    void undo() override;
    std::string describe() const override;
private:
    ResponseUnit& unit_;
    Incident&incident_;
};

class LockAreaCommand : public Command {
public:
    LockAreaCommand(AccessControlSystem&access, const std::string& area, AccessMode mode);
    void execute() override;
    void undo() override;
    std::string describe() const override;

private:
    AccessControlSystem& access_;
    std::string area_;
    AccessMode mode_;
    AccessMode previous_;
};


class IssueAlertCommand : public Command {
public:
    IssueAlertCommand(ExternalAlertService& alerts, const std::string& area,Severity severity, const std::string& message);
    void execute() override;
    void undo() override;
    std::string describe() const override;
private:
    ExternalAlertService& alerts_;
    std::string area_;
    Severity severity_;
    std::string message_;
};


class EvacuateAreaCommand : public Command {
public:
    EvacuateAreaCommand(AccessControlSystem& access, ExternalAlertService& alerts,const std::string& area);
    void execute() override;
    void undo() override;
    std::string describe() const override;

private:
    AccessControlSystem&access_;
    ExternalAlertService&alerts_;
    std::string area_;
    AccessMode previous_;
};

#endif
