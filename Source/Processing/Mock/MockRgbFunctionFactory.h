/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Mock RGB function factory.
 */

#ifndef PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_
#define PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_

#include <gmock/gmock.h>

#include "../Interfaces/IRgbFunctionFactory.h"

class MockRgbFunctionFactory
    : public IRgbFunctionFactory
{
public:
    MOCK_CONST_METHOD1(createRgbFunction, IRgbFunction*(json converted));
};



#endif /* PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_ */
