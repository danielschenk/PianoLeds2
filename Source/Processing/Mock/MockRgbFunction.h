/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Mock RGB function.
 */

#ifndef PROCESSING_MOCK_MOCKRGBFUNCTION_H_
#define PROCESSING_MOCK_MOCKRGBFUNCTION_H_

#include <gmock/gmock.h>

#include "../Interfaces/IRgbFunction.h"

class MockRgbFunction
    : public IRgbFunction
{
public:
    MOCK_CONST_METHOD2(calculate, Processing::TRgb(const Processing::TNoteState& noteState, Processing::TTime currentTime));
    MOCK_CONST_METHOD0(convertToJson, json());
    MOCK_METHOD1(convertFromJson, void(json converted));
};


#endif /* PROCESSING_MOCK_MOCKRGBFUNCTION_H_ */
