#include "AccessControlSystem.h"
#include <iostream>

void AccessControlSystem::addArea(const std::string& area) {
    modes_[area] = AccessMode::Unlocked;
    std::cout << "[Access] registered area " << area << " (Unlocked)\n";
}

bool AccessControlSystem::hasArea(const std::string& area) const {
    return modes_.find(area) != modes_.end();
}

bool AccessControlSystem::setMode(const std::string& area, AccessMode mode) {
    std::map<std::string, AccessMode>::iterator it = modes_.find(area);
    if (it == modes_.end()) {
        std::cout << "[Access] unknown area '" << area << "'\n";
        return false;
    }
    std::cout << "[Access] " << area << ": " << toString(it->second)<< " -> " << toString(mode) << "\n";
    it->second = mode;
    return true;
}

AccessMode AccessControlSystem::modeOf(const std::string& area) const {
    return modes_.at(area);      
}
