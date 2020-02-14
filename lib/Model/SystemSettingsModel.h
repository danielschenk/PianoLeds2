/**
 * @file
 *
 * MIT License
 *
 * @copyright (c) 2019 Daniel Schenk <danielschenk@users.noreply.github.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SYSTEMSETTINGSMODEL_H
#define SYSTEMSETTINGSMODEL_H

#include "Model.h"

class SystemSettingsModel: public Model
{
public:
    /**
     * Constructor
     */
    SystemSettingsModel() = default;

    // Prevent implicit copy constructor & assignment operator
    SystemSettingsModel(const SystemSettingsModel&) = delete;
    SystemSettingsModel& operator=(const SystemSettingsModel&) = delete;

    std::string getWifiAPSsid() const;
    void setWifiAPSsid(std::string wifiStationSsid);
    std::string getWifiAPPassword() const;
    void setWifiAPPassword(std::string wifiStationPassword);

private:
    /**
     * Name of the WiFi network when in AP mode
     */
    std::string wifiAPSsid = "PianoLeds";

    /**
     * Password of the WiFi network when in AP mode
     */
    std::string wifiAPPassword = "LedsFlashSomeNotes";
};


#endif //SYSTEMSETTINGSMODEL_H
