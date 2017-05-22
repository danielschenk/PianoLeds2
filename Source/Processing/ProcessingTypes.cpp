/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
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

} /* namespace Processing */


