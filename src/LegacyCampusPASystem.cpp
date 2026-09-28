#include "LegacyCampusPASystem.h"
#include <cstring>
#include <iostream>

void LegacyCampusPASystem::PA_RegisterZone(int zoneCode) {
    amplifierOn_[zoneCode] = 1;
}

void LegacyCampusPASystem::PA_SetAmplifier(int zoneCode, int on) {

    std::map<int, int>::iterator it = amplifierOn_.find(zoneCode);

    if (it == amplifierOn_.end()) return;

    it->second = on ? 1 : 0;

    std::cout << "[LegacyPA] zone " << zoneCode << " amplifier "<< (on ? "ONLINE" : "OFFLINE") << "\n";
}

int LegacyCampusPASystem::PA_Broadcast(int zoneCode, int priority, const char*text) {
    std::map<int, int>::const_iterator it = amplifierOn_.find(zoneCode);
    if (it == amplifierOn_.end())return -1;
    if (!it->second)return -2;
    if (priority < 1 || priority > 5)return -3;
    if (text == 0 || std::strlen(text) > 60)return -4;
    std::cout << "[LegacyPA] ZONE " << zoneCode << " PRI " << priority<< " >> " << text << "\n";
    return 0;
}
