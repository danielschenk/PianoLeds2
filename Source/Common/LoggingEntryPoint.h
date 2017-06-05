/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Logging entry point class.
 */

#ifndef COMMON_LOGGINGENTRYPOINT_H_
#define COMMON_LOGGINGENTRYPOINT_H_

#include <vector>
#include <mutex>
#include <cstdint>

#include "Interfaces/LoggingDefinitions.h"

class ILoggingTarget;

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
     * @param[in]   rSubscriber Reference to the subscriber.
     */
    static void subscribe(ILoggingTarget& rSubscriber);

    /**
     * Unsubscribe from logging events.
     *
     * @param[in]   rSubscriber Reference to the subscriber.
     */
    static void unsubscribe(ILoggingTarget& rSubscriber);

    /**
     * Log a message.
     *
     * @param[in]   time        Timestamp of the log call.
     * @param[in]   level       Log level.
     * @param[in]   component   Originating component.
     * @param[in]   fmt         Format string of the log message.
     * @param[in]   ...         Arguments to use for string formatting.
     */
    static void logMessage(uint64_t time, Logging::TLogLevel level, const char *component, const char *fmt, ...) __attribute__((format (printf, 4, 5)));

private:
    /** Max log message size, excluding file, line and level information. */
    static constexpr unsigned int c_maxMessageSize = 256;

    /** The list of subscribers. */
    static std::vector<ILoggingTarget*> s_subscribers;

    /** Mutex to protect the list of subscribers. */
    static std::mutex s_mutex;
};

#endif /* COMMON_LOGGINGENTRYPOINT_H_ */
