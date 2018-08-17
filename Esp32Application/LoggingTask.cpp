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

#include <cstdio>
#include <Stream.h>

#include "Common/LoggingEntryPoint.h"
#include "LoggingTask.h"

LoggingTask::LoggingTask(Stream& rSerial,
                         uint32_t stackSize,
                         UBaseType_t priority)
    : BaseTask("logging", stackSize, priority)
    , m_rSerial(rSerial)
{
    m_queue = xQueueCreate(10, sizeof(QueueEntry));
    LoggingEntryPoint::subscribe(*this);
}

LoggingTask::~LoggingTask()
{
    LoggingEntryPoint::unsubscribe(*this);
    vQueueDelete(m_queue);
}

void LoggingTask::logMessage(uint64_t time,
                             Logging::TLogLevel level,
                             std::string component,
                             std::string message)
{
    QueueEntry entry;
    entry.time = time;
    entry.level = level;
    entry.pComponent = new std::string(component);
    entry.pMessage = new std::string(message);

    xQueueSend(m_queue, &entry, portMAX_DELAY);
}

void LoggingTask::run()
{
    QueueEntry entry;

    // Wait for item forever
    while(xQueueReceive(m_queue, &entry, portMAX_DELAY) == pdTRUE)
    {
        char buf[256];

        const char* pLevelString;
        switch(entry.level)
        {
        case Logging::LogLevel_Debug:
            pLevelString = "Debug";
            break;

        case Logging::LogLevel_Info:
            pLevelString = "Info";
            break;

        case Logging::LogLevel_Warning:
            pLevelString = "Warning";
            break;

        case Logging::LogLevel_Error:
        default:
            pLevelString = "Error";
            break;
        }

        snprintf(buf, sizeof(buf), "%llu %s(%s): %s",
                 entry.time,
                 pLevelString,
                 entry.pComponent->c_str(),
                 entry.pMessage->c_str());

        delete entry.pComponent;
        delete entry.pMessage;

        m_rSerial.print(buf);
    }
}
