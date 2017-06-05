/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Interface for logging targets.
 */

#ifndef COMMON_INTERFACES_ILOGGINGTARGET_H_
#define COMMON_INTERFACES_ILOGGINGTARGET_H_

#include <string>
#include <cstdint>

#include "LoggingDefinitions.h"

/**
 * Interface for logging targets.
 */
class ILoggingTarget
{
public:
    /**
     * Destructor.
     */
    virtual ~ILoggingTarget()
    {
    }

    /**
     * Log a message.
     *
     * @param[in]   time        Timestamp of the log call.
     * @param[in]   level       Log level.
     * @param[in]   component   Originating component.
     * @param[in]   message     The log message.
     */
    virtual void logMessage(uint64_t time, Logging::TLogLevel level, std::string component, std::string message) = 0;
};



#endif /* COMMON_INTERFACES_ILOGGINGTARGET_H_ */
