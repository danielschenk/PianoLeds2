/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Interface for RGB function factories.
 */

#ifndef PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_
#define PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_

#include <json.hpp>
using json = nlohmann::json;

class IRgbFunction;

/**
 * Interface for RGB function factories.
 */
class IRgbFunctionFactory
{
public:
    /**
     * Destructor.
     */
    virtual ~IRgbFunctionFactory()
    {
    }

    /**
     * Create RGB function from JSON input.
     *
     * @param   [in]    json    The JSON object containing the type name and persistent properties.
     *
     * @return  The newly created RGB function or nullptr if type could not be determined.
     */
    virtual IRgbFunction* createRgbFunction(json json) const = 0;
};



#endif /* PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_ */
