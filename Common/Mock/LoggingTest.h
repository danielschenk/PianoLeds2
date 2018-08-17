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
