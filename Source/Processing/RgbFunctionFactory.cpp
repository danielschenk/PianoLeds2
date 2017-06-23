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
 */

#include "RgbFunctionFactory.h"
#include "LinearRgbFunction.h"
#include "Interfaces/IJsonConvertible.h"
#include "Interfaces/IRgbFunction.h"

IRgbFunction* RgbFunctionFactory::createRgbFunction(json json) const
{
    IRgbFunction* rgbFunction = nullptr;
    if(json.count(IJsonConvertible::c_objectTypeKey) > 0)
    {
        std::string objectType = json[IJsonConvertible::c_objectTypeKey];
        if(objectType == IRgbFunction::c_jsonTypeNameLinearRgbFunction)
        {
            rgbFunction = new LinearRgbFunction();
        }

        if(rgbFunction != nullptr)
        {
            rgbFunction->convertFromJson(json);
        }
    }

    return rgbFunction;
}
