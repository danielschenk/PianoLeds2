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
 * @brief Mock logging target.
 */

#ifndef COMMON_MOCK_MOCKLOGGINGTARGET_H_
#define COMMON_MOCK_MOCKLOGGINGTARGET_H_

#include <gmock/gmock.h>

#include "../Interfaces/ILoggingTarget.h"

class MockLoggingTarget
    : public ILoggingTarget
{
public:
    // ILoggingTarget implementation
    MOCK_METHOD4(logMessage, void(uint64_t time, Logging::TLogLevel level, std::string component, std::string message));
};



#endif /* COMMON_MOCK_MOCKLOGGINGTARGET_H_ */
