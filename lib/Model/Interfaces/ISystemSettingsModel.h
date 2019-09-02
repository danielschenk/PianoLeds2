//
// Created by Daniël Schenk on 2019-09-03.
//

#ifndef ISYSTEMSETTINGSMODEL_H
#define ISYSTEMSETTINGSMODEL_H

#include <string>

class ISystemSettingsModel
{
public:
    /**
     * Destructor.
     */
    virtual ~ISystemSettingsModel() = default;
    
    virtual std::string getWifiStationSsid() const = 0;
    virtual void setWifiStationSsid(std::string wifiStationSsid) = 0;
    virtual std::string getWifiStationPassword() const = 0;
    virtual void setWifiStationPassword(std::string wifiStationPassword) = 0;
};


#endif //ISYSTEMSETTINGSMODEL_H
