#include "OperatorConsole.h"
#include <exception>
#include <iostream>
#include <utility>

bool OperatorConsole::submit(std::unique_ptr<Command> command) {
    if (!command) return false;
    std::cout<< "[Console] submit: "<< command->describe()<<"\n";

    try {
        command->execute();
    } 
    catch (const std::exception& e) {
        std::cout << "[Console] REJECTED: "<< command->describe()<< " -- " << e.what()<<"\n";
        return false;
    }

    history_.push_back(std::move(command));
    std::cout<< "[Console] recorded (history: " << history_.size()<< ")\n";
    return true;
}

bool OperatorConsole::cancelLast() {
    if (history_.empty()) {
        std::cout << "[Console] nothing to cancel\n";
        return false;
    }

    std::unique_ptr<Command> last = std::move(history_.back());
    history_.pop_back();
    std::cout<<"[Console] cancel: " << last->describe() << "\n";
    try {
        last->undo();
    } 
    catch (const std::exception& e) {
        std::cout << "[Console] cancel FAILED: "<< e.what()<<"\n";
        return false;
    }
    return true;
}
