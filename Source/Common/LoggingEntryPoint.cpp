/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 */

#include <vector>
#include <string>
#include <mutex>
#include <cstdarg>
#include <cstdio>

#include "LoggingEntryPoint.h"
#include "Interfaces/ILoggingTarget.h"

std::vector<ILoggingTarget*> LoggingEntryPoint::s_subscribers;
std::mutex LoggingEntryPoint::s_mutex;

void LoggingEntryPoint::subscribe(ILoggingTarget& rSubscriber)
{
    std::lock_guard<std::mutex> lock(s_mutex);
    bool found(false);
    for(auto pLoggingTarget : s_subscribers)
    {
        if(pLoggingTarget == &rSubscriber)
        {
            found = true;
            break;
        }
    }
    if(!found)
    {
        s_subscribers.push_back(&rSubscriber);
    }
}

void LoggingEntryPoint::unsubscribe(ILoggingTarget& rSubscriber)
{
    std::lock_guard<std::mutex> lock(s_mutex);
    s_subscribers.erase(std::remove(s_subscribers.begin(), s_subscribers.end(), &rSubscriber), s_subscribers.end());
}

void LoggingEntryPoint::logMessage(uint64_t time, Logging::TLogLevel level, const char *component, const char *fmt, ...)
{
    std::lock_guard<std::mutex> lock(s_mutex);
    if(s_subscribers.size() > 0)
    {
        std::vector<char> buffer(c_maxMessageSize);
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(buffer.data(), buffer.size(), fmt, args);
        va_end(args);

        std::string message(buffer.data());
        std::string componentStr(component);
        for(auto pLoggingTarget : s_subscribers)
        {
            if (pLoggingTarget != nullptr)
            {
                pLoggingTarget->logMessage(time, level, componentStr, message);
            }
        }
    }
}
