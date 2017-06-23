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
 * @brief Basic fixture for tests which test a component that logs stuff.
 */

#ifndef COMMON_MOCK_LOGGINGTEST_H_
#define COMMON_MOCK_LOGGINGTEST_H_

#include <gtest/gtest.h>

#include "MockLoggingTarget.h"
#include "../LoggingEntryPoint.h"

using ::testing::AnyOf;
using ::testing::_;
using ::testing::AnyNumber;

class LoggingTest
    : public ::testing::Test
{
public:
    LoggingTest()
        : m_mockLoggingTarget()
    {
        // Info and debug logs are OK
        EXPECT_CALL(m_mockLoggingTarget, logMessage(_, AnyOf(Logging::LogLevel_Info, Logging::LogLevel_Debug), _, _))
            .Times(AnyNumber());

        LoggingEntryPoint::subscribe(m_mockLoggingTarget);
    }

    virtual ~LoggingTest()
    {
        LoggingEntryPoint::unsubscribe(m_mockLoggingTarget);
    }

    MockLoggingTarget m_mockLoggingTarget;
};


#endif /* COMMON_MOCK_LOGGINGTEST_H_ */
