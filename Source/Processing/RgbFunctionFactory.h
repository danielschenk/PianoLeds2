/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Factory for RGB functions.
 */

#ifndef PROCESSING_RGBFUNCTIONFACTORY_H_
#define PROCESSING_RGBFUNCTIONFACTORY_H_

#include <json.hpp>
using json = nlohmann::json;

class IRgbFunction;

/**
 * Factory for RGB functions.
 */
class RgbFunctionFactory
{
public:
    // Prevent implicit constructor, copy constructor and assignment operator
    RgbFunctionFactory() = delete;
    RgbFunctionFactory(const RgbFunctionFactory&) = delete;
    RgbFunctionFactory& operator=(const RgbFunctionFactory&) = delete;

    /**
     * Create RGB function from JSON input.
     *
     * @param   [in]    json    The JSON object containing the type name and persistent properties.
     *
     * @return  The newly created RGB function or nullptr if type could not be determined.
     */
    static IRgbFunction* createRgbFunction(json json);
};

#endif /* PROCESSING_RGBFUNCTIONFACTORY_H_ */
