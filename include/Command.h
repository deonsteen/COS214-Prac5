#ifndef COMMAND_H
#define COMMAND_H
#include <string>

class Command {
public:
    virtual ~Command() {

    }
    virtual void execute() = 0; // ec on screwup
    virtual void undo() = 0;
    virtual std::string describe() const = 0;
};

#endif
