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
 * @brief Processing type implementations.
 */

#include <cstdint>

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

TStringNoteToLightMap convert(const TNoteToLightMap& rSource)
{
    TStringNoteToLightMap converted;
    for(const auto& rPair : rSource)
    {
        converted[std::to_string(rPair.first)] = rPair.second;
    }

    return converted;
}

TNoteToLightMap convert(const TStringNoteToLightMap& rSource)
{
    TNoteToLightMap converted;
    for(unsigned int noteNumber = 0; noteNumber <= UINT8_MAX; ++noteNumber)
    {
        std::string key = std::to_string(noteNumber);
        if(rSource.count(key) > 0)
        {
            converted[noteNumber] = rSource.at(key);
        }
    }

    return converted;
}

} /* namespace Processing */


