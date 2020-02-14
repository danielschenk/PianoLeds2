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

#include "PianoDecayRgbFunction.h"
#include <json11.hpp>

Processing::TRgb PianoDecayRgbFunction::calculate(const Processing::TNoteState& noteState,
                                                  Processing::TTime currentTime) const
{
    auto startColor(LinearRgbFunction::calculate(noteState, currentTime));
    if(startColor == Processing::TRgb{0, 0, 0})
    {
        return startColor;
    }

    uint32_t soundingTime(currentTime - noteState.noteOnTimeStamp);

    float timeProgress, decayFactor, startIntensityFactor;
    if(soundingTime < c_fastDecayDurationMs)
    {
        timeProgress = static_cast<float>(soundingTime) / static_cast<float>(c_fastDecayDurationMs);
        decayFactor = c_fastDecayFactor;
        startIntensityFactor = 1.0f;
    }
    else
    {
        // Note timeProgress starts again from 0 here, so 0 means at 1200ms from note press down time!
        timeProgress = static_cast<float>(soundingTime - c_fastDecayDurationMs) / static_cast<float>(c_slowDecayDurationMs);
        decayFactor = c_slowDecayFactor;
        // We start at the intensity factor where we left off in the first phase
        startIntensityFactor = 1.0f - c_fastDecayFactor;
    }

    float intensityFactor(startIntensityFactor - (timeProgress * decayFactor));
    return startColor * intensityFactor;
}

std::string PianoDecayRgbFunction::getObjectType() const
{
    return IRgbFunction::c_jsonTypeNamePianoDecayRgbFunction;
}
