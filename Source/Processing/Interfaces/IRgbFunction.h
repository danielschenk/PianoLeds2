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
 * @brief Interface for RGB functions.
 */

#ifndef PROCESSING_IRGBFUNCTION_H_
#define PROCESSING_IRGBFUNCTION_H_

#include "IJsonConvertible.h"
#include "ProcessingTypes.h"

/**
 * Interface for RGB functions.
 */
class IRgbFunction
    : public IJsonConvertible
{
public:
    static constexpr const char* c_jsonTypeNameLinearRgbFunction = "LinearRgbFunction";

    /**
     * Destructor.
     */
    virtual ~IRgbFunction()
    {
    };

    /**
     * Calculate the output color based on the given note state and current time.
     *
     * @param[in]   noteState   The note state.
     * @param[in]   currentTime The current time.
     *
     * @return  The output color.
     */
    virtual Processing::TRgb calculate(const Processing::TNoteState& noteState, Processing::TTime currentTime) const = 0;
};

#endif /* PROCESSING_IRGBFUNCTION_H_ */
