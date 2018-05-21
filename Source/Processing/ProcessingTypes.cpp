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
 * @brief Processing type implementations.
 */

#include <cstdint>
#include <stdio.h>

#include <Common/Utilities/Json11Helper.h>

#include "Interfaces/ProcessingTypes.h"

namespace Processing
{

TRgb::TRgb(uint8_t initialR, uint8_t initialG, uint8_t initialB)
{
    r = initialR;
    g = initialG;
    b = initialB;
};

TRgb::TRgb()
{
    r = 0;
    g = 0;
    b = 0;
};

bool TRgb::operator ==(const TRgb& other) const
{
    return (other.r == r) &&
           (other.g == g) &&
           (other.b == b);
}

TRgb TRgb::operator *(float factor) const
{
    return {
        (uint8_t)(factor * (float)r),
        (uint8_t)(factor * (float)g),
        (uint8_t)(factor * (float)b)
    };
}

TRgb operator*(float factor, const TRgb& color)
{
    return {
        (uint8_t)(factor * (float)color.r),
        (uint8_t)(factor * (float)color.g),
        (uint8_t)(factor * (float)color.b)
    };
}

/**
 * Construct color from float values, clamping if necessary.
 */
TRgb rgbFromFloat(float initialR, float initialG, float initialB)
{
    if(initialR > UINT8_MAX) {initialR = UINT8_MAX;}
    if(initialR <         0) {initialR =         0;}
    if(initialG > UINT8_MAX) {initialG = UINT8_MAX;}
    if(initialG <         0) {initialG =         0;}
    if(initialB > UINT8_MAX) {initialB = UINT8_MAX;}
    if(initialB <         0) {initialB =         0;}

    return TRgb((uint8_t)initialR, (uint8_t)initialG, (uint8_t)initialB);
}

Json convert(const TNoteToLightMap& rSource)
{
    Json::object converted;
    for(const auto& rPair : rSource)
    {
        char buf[4];
        snprintf(buf, sizeof(buf), "%u", rPair.first);
        converted[std::string(buf)] = Json(rPair.second);
    }

    return Json(converted);
}

TNoteToLightMap convert(const Json& rSource)
{
    Json11Helper helper(__PRETTY_FUNCTION__, rSource, false /* logMissingKeys */);

    TNoteToLightMap converted;
    for(unsigned int noteNumber = 0; noteNumber <= UINT8_MAX; ++noteNumber)
    {
        char buf[4];
        snprintf(buf, sizeof(buf), "%u", noteNumber);

        uint8_t lightNumber;
        if(helper.getItemIfPresent(std::string(buf), lightNumber))
        {
            converted[noteNumber] = lightNumber;
        }
    }

    return converted;
}

} /* namespace Processing */


