/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2018 Daniel Schenk <danielschenk@users.noreply.github.com>
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

#ifndef ESP32APPLICATION_LOGGINGTASK_H_
#define ESP32APPLICATION_LOGGINGTASK_H_

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "ILoggingTarget.h"
#include "BaseTask.h"

class Stream;

/**
 * The logging task.
 */
class LoggingTask
    : public ILoggingTarget
    , public BaseTask
{
public:
    /**
     * Constructor.
     *
     * @param serial    The Arduino Serial instance to use
     * @param stackSize Stack size in words
     * @param priority  Priority
     */
    LoggingTask(Stream& serial,
                uint32_t stackSize,
                UBaseType_t priority);

    /**
     * Destructor.
     */
    ~LoggingTask() override;

    // Prevent implicit constructors and assignment operator
    LoggingTask() = delete;
    LoggingTask(const LoggingTask&) = delete;
    LoggingTask& operator=(const LoggingTask&) = delete;

    // ILoggingTarget implementation
    void logMessage(uint64_t time, Logging::TLogLevel level, std::string component, std::string message) override;

private:
    struct QueueEntry
    {
        uint64_t time;
        Logging::TLogLevel level;
        std::string* component;
        std::string* message;
    };

    void run() override;

    Stream& m_serial;
    QueueHandle_t m_queue;
};

#endif /* ESP32APPLICATION_LOGGINGTASK_H_ */
