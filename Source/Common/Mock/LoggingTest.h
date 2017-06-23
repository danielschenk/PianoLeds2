/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
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
