#ifndef EXTERNAL_ALERT_SERVICE_H
#define EXTERNAL_ALERT_SERVICE_H
#include <string>
#include "Types.h"

class ExternalAlertService {
public:
    virtual ~ExternalAlertService() {
        
    }
    virtual bool sendAlert(const std::string& area, Severity severity,const std::string& message) = 0;// if its true dell
};
#endif
