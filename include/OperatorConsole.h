#ifndef OPERATOR_CONSOLE_H
#define OPERATOR_CONSOLE_H
#include <cstddef>
#include <memory>
#include <vector>
#include "Command.h"

class OperatorConsole {
public:
    bool submit(std::unique_ptr<Command> command);
    bool cancelLast();
    std::size_t historySize() const { 
        return history_.size(); 
    }

private:
    std::vector<std::unique_ptr<Command>> history_;   
};
#endif
