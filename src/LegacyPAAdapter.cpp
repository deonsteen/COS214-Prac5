#include "LegacyPAAdapter.h"
#include <iostream>

namespace {

const std::size_t kMaxChunk=60;

int priorityFor(Severity severity) {
    static const int priorities[] ={1, 3, 5};          
    return priorities[static_cast<int>(severity)];
}

const char* explain(int code) {
    static const char* reasons[]={ "ok", "unknown zone", "amplifier offline","invalid priority", "text too long" };
    int index=-code;
    return (index>=0&&index<=4) ? reasons[index] : "unrecognised error";
}
}

void LegacyPAAdapter::mapArea(const std::string&area, int zoneCode) {
    zones_[area]=zoneCode;
    pa_.PA_RegisterZone(zoneCode);
}

int LegacyPAAdapter::zoneFor(const std::string&area) const {
    std::map<std::string, int>::const_iterator it = zones_.find(area);
    return it==zones_.end() ? -1 : it->second;
}
bool LegacyPAAdapter::sendAlert(const std::string& area, Severity severity,const std::string& message) {
    int zone=zoneFor(area);
    if (zone<0) {
        std::cout<< "[Adapter] no PA zone mapped for area '" << area << "'\n";
        return false;
    }
    int priority = priorityFor(severity);
    std::size_t chunks =message.empty() ? 1 : (message.size()+kMaxChunk- 1) / kMaxChunk;
    std::cout << "[Adapter] '" << area << "' -> zone " << zone << ", " << toString(severity)<< " -> priority " << priority << ", " << chunks << " chunk(s)\n";

    std::size_t pos= 0;
    do {
        std::string chunk =message.substr(pos, kMaxChunk);
        int code=pa_.PA_Broadcast(zone, priority, chunk.c_str());
        if (code != 0) {
            std::cout << "[Adapter] legacy PA returned " << code << " (" << explain(code)<< ") -> alert NOT delivered\n";
            return false;
        }

        pos+=kMaxChunk;
    } 
    while (pos<message.size());
    return true;
}
