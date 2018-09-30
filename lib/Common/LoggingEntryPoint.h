/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 * 
 * @brief Logging entry point class.
 */

#ifndef COMMON_LOGGINGENTRYPOINT_H_
#define COMMON_LOGGINGENTRYPOINT_H_

#include <vector>
#include <mutex>
#include <cstdint>

#include "LoggingDefinitions.h"

class ILoggingTarget;
class ITime;

/**
 * Static class which receives log events and distributes them to subscribers (logging implementations).
 */
class LoggingEntryPoint
{
public:
    // Prevent implicit constructor, copy constructor and assignment operator.
    LoggingEntryPoint() = delete;
    LoggingEntryPoint(const LoggingEntryPoint&) = delete;
    LoggingEntryPoint& operator =(const LoggingEntryPoint&) = delete;

    /**
     * Subscribe to logging events.
     *
     * @param[in]   subscriber  Reference to the subscriber.
     */
    static void subscribe(ILoggingTarget& subscriber);

    /**
     * Unsubscribe from logging events.
     *
     * @param[in]   subscriber  Reference to the subscriber.
     */
    static void unsubscribe(ILoggingTarget& subscriber);

    /**
     * Set the time provider.
     */
    static void setTime(const ITime* time);

    /**
     * Log a message.
     *
     * @param[in]   level       Log level.
     * @param[in]   component   Originating component.
     * @param[in]   fmt         Format string of the log message.
     * @param[in]   ...         Arguments to use for string formatting.
     */
    static void logMessage(Logging::TLogLevel level, const char *component, const char *fmt, ...) __attribute__((format (printf, 3, 4)));

    /** Max log message size, excluding file, line and level information. */
    static constexpr unsigned int c_maxMessageSize = 2048;
private:
    /** The list of subscribers. */
    static std::vector<ILoggingTarget*> s_subscribers;

    /** Mutex to protect the list of subscribers. */
    static std::mutex s_mutex;

    static const ITime* s_time;
};

#endif /* COMMON_LOGGINGENTRYPOINT_H_ */
