/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
     *
     * @return  The output color.
     */
    virtual Processing::TRgb calculate(const Processing::TNoteState& noteState) const = 0;
};

#endif /* PROCESSING_IRGBFUNCTION_H_ */
