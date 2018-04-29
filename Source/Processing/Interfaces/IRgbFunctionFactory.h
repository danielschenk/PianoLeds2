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
 * @brief Interface for RGB function factories.
 */

#ifndef PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_
#define PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_

#include <nlohmann/json.hpp>
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
