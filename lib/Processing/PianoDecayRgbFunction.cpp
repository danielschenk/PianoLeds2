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

#include <json11.hpp>

#include "PianoDecayRgbFunction.h"

PianoDecayRgbFunction::PianoDecayRgbFunction()
    : m_color()
    , m_mutex()
{
}

Processing::TRgb PianoDecayRgbFunction::calculate(const Processing::TNoteState& noteState,
                                                  Processing::TTime currentTime) const
{
    if(!noteState.sounding)
    {
        return Processing::TRgb({0, 0, 0});
    }

    uint32_t soundingTime(currentTime - noteState.noteOnTimeStamp);

    float divider, decayFactor, start;
    if(soundingTime < c_fastDecayEndTimeMs)
    {
        divider = static_cast<float>(c_fastDecayEndTimeMs);
        decayFactor = c_fastDecayFactor;
        start = 1.0f;
    }
    else
    {
        divider = static_cast<float>(c_slowDecayEndTimeMs);
        decayFactor = c_slowDecayFactor;
        start = 1.0f - c_fastDecayFactor;
    }

    float intensityFactor(start - (static_cast<float>(soundingTime) / divider) * decayFactor);

    std::lock_guard<std::mutex> lock(m_mutex);
    return (static_cast<float>(noteState.pressDownVelocity) / 255.0f) * intensityFactor * m_color;
}

void PianoDecayRgbFunction::setColor(Processing::TRgb color)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_color = color;
}

Json PianoDecayRgbFunction::convertToJson() const
{
    // TODO
    return Json::object();
}

void PianoDecayRgbFunction::convertFromJson(const Json& converted)
{
    // TODO
}

std::string PianoDecayRgbFunction::getObjectType() const
{
    // TODO
    return "";
}
