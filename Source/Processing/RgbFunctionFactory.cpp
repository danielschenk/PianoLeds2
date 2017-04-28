/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 */

#include "RgbFunctionFactory.h"
#include "LinearRgbFunction.h"
#include "Interfaces/IJsonConvertible.h"
#include "Interfaces/IRgbFunction.h"

IRgbFunction* RgbFunctionFactory::create(json json)
{
    IRgbFunction* rgbFunction = nullptr;
    if(json.count(IJsonConvertible::c_objectTypeKey) > 0)
    {
        std::string objectType = json[IJsonConvertible::c_objectTypeKey];
        if(objectType == IRgbFunction::c_jsonTypeNameLinearRgbFunction)
        {
            rgbFunction = new LinearRgbFunction();
            if(rgbFunction != nullptr)
            {
                rgbFunction->convertFromJson(json);
            }
        }
    }

    return rgbFunction;
}
