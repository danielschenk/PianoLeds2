/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
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
