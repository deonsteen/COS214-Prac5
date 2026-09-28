#ifndef LEGACY_PA_ADAPTER_H
#define LEGACY_PA_ADAPTER_H
#include <map>
#include <string>
#include "ExternalAlertService.h"
#include "LegacyCampusPASystem.h"

class LegacyPAAdapter : public ExternalAlertService {
public:
    explicit LegacyPAAdapter(LegacyCampusPASystem& pa) : pa_(pa) {
    }
    void mapArea(const std::string& area, int zoneCode);
    int zoneFor(const std::string& area) const;      
    bool sendAlert(const std::string& area, Severity severity,const std::string& message) override;
private:
    LegacyCampusPASystem& pa_;// naw
    std::map<std::string, int> zones_;
};

#endif
