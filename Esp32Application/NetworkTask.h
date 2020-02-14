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

#ifndef NETWORKTASK_H
#define NETWORKTASK_H

#include "BaseTask.h"
#include <WiFi.h>
#include <freertos/FreeRTOS.h>

class SystemSettingsModel;

class NetworkTask: public BaseTask
{
public:
    /**
     * Constructor.
     *
     * @param systemSettingsModel   The system settings model to get WiFi settings from
     * @param stackSize             The stack size to use for the task, in words
     * @param priority              The priority to use for the task
     */
    NetworkTask(const SystemSettingsModel& systemSettingsModel,
                uint32_t stackSize,
                UBaseType_t priority);

    /**
     * Destructor.
     */
    ~NetworkTask() override;

    // Prevent default constructor, copy constructor and assignment operator
    NetworkTask() = delete;
    NetworkTask(const NetworkTask&) = delete;
    NetworkTask& operator=(const NetworkTask&) = delete;

private:
    // BaseTask implementation
    void run() override;

    const SystemSettingsModel& systemSettingsModel;
    unsigned int systemSettingsModelSubscription;
};


#endif //NETWORKTASK_H
