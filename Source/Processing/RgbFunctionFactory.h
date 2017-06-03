/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Factory for RGB functions.
 */

#ifndef PROCESSING_RGBFUNCTIONFACTORY_H_
#define PROCESSING_RGBFUNCTIONFACTORY_H_

#include "Interfaces/IRgbFunctionFactory.h"

/**
 * Factory for RGB functions.
 */
class RgbFunctionFactory
    : public IRgbFunctionFactory
{
public:
    // Prevent implicit constructor, copy constructor and assignment operator
    RgbFunctionFactory() = delete;
    RgbFunctionFactory(const RgbFunctionFactory&) = delete;
    RgbFunctionFactory& operator=(const RgbFunctionFactory&) = delete;

    // IRgbFunctionFactory implementation
    virtual IRgbFunction* createRgbFunction(json json) const;
};

#endif /* PROCESSING_RGBFUNCTIONFACTORY_H_ */
