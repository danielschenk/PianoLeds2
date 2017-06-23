/**
 * @file
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
 * This file is part of MLC2.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
