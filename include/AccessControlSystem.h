#ifndef ACCESS_CONTROL_SYSTEM_H
#define ACCESS_CONTROL_SYSTEM_H
#include <map>
#include <string>
#include "Types.h"

class AccessControlSystem {

public:
    void addArea(const std::string&area);                  
    bool hasArea(const std::string&area) const;
    bool setMode(const std::string&area, AccessMode mode); 
    AccessMode modeOf(const std::string&area) const;       

private:
    std::map<std::string, AccessMode> modes_;
};
#endif
