#ifndef LEGACY_CAMPUS_PA_SYSTEM_H
#define LEGACY_CAMPUS_PA_SYSTEM_H
#include <map>

class LegacyCampusPASystem {
public:
    void PA_RegisterZone(int zoneCode);                
    void PA_SetAmplifier(int zoneCode, int on);
    int  PA_Broadcast(int zoneCode, int priority, const char*text);
private:
    std::map<int, int> amplifierOn_;
};
#endif
