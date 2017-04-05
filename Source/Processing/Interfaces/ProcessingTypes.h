/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Processing type definitions.
 */

#ifndef PROCESSING_INTERFACES_PROCESSINGTYPES_H_
#define PROCESSING_INTERFACES_PROCESSINGTYPES_H_

#include <cstdint>

namespace Processing
{
/** Type for single RGB color. */
typedef struct TRgb
{
    TRgb(uint8_t initialR, uint8_t initialG, uint8_t initialB)
    {
        r = initialR;
        g = initialG;
        b = initialB;
    };

    TRgb()
    {
        r = 0;
        g = 0;
        b = 0;
    };

    bool operator==(const TRgb& other) const
    {
        return (other.r == r) &&
               (other.g == g) &&
               (other.b == b);
    }

    uint8_t r;
    uint8_t g;
    uint8_t b;
} TRgb;

/** Type for RGB strip data. */
typedef std::vector<TRgb> TRgbStrip;
}


#endif /* PROCESSING_INTERFACES_PROCESSINGTYPES_H_ */
