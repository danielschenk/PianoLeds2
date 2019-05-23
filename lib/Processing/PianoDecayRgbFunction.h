/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2018 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 */

#ifndef LIB_PROCESSING_PIANODECAYRGBFUNCTION_H_
#define LIB_PROCESSING_PIANODECAYRGBFUNCTION_H_

#include <stdint.h>
#include <mutex>

#include "IRgbFunction.h"
#include "ProcessingTypes.h"

/**
 * RGB function which slowly dims sounding notes to visually mimic the sound of piano strings.
 */
class PianoDecayRgbFunction : public IRgbFunction
{
public:
    /**
     * Constructor.
     */
    PianoDecayRgbFunction();

    // Prevent implicit constructor and assigment operator
    PianoDecayRgbFunction(const PianoDecayRgbFunction&) = delete;
    PianoDecayRgbFunction& operator=(const PianoDecayRgbFunction&) = delete;

    /**
     * Destructor.
     */
    virtual ~PianoDecayRgbFunction() = default;

    void setColor(Processing::TRgb color);

    // IRgbFunction implementation
    Processing::TRgb calculate(const Processing::TNoteState& noteState, Processing::TTime currentTime) const override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

protected:
    // IRgbFunction implementation
    std::string getObjectType() const override;

private:
    static constexpr uint32_t c_fastDecayEndTimeMs = 1200;
    static constexpr float c_fastDecayFactor = 0.5f;

    static constexpr uint32_t c_slowDecayEndTimeMs = 15000;
    static constexpr float c_slowDecayFactor = 0.5f;

    Processing::TRgb m_color;
    mutable std::mutex m_mutex;
};

#endif /* LIB_PROCESSING_PIANODECAYRGBFUNCTION_H_ */
