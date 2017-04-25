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
#include <map>
#include <algorithm>

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

    /**
     * Compare with another @ref TRgb.
     */
    bool operator==(const TRgb& other) const
    {
        return (other.r == r) &&
               (other.g == g) &&
               (other.b == b);
    }

    /**
     * Multiply every color by a single factor.
     */
    TRgb operator*(float factor) const
    {
        return {
            (uint8_t)(factor * (float)r),
            (uint8_t)(factor * (float)g),
            (uint8_t)(factor * (float)b)
        };
    }

    uint8_t r;
    uint8_t g;
    uint8_t b;
} TRgb;

/**
 * Multiply every color by a single factor.
 */
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

/** Type for RGB strip data. */
typedef std::vector<TRgb> TRgbStrip;

/** Type to map MIDI note numbers to lights. */
typedef std::map<uint8_t, uint8_t> TNoteToLightMap;

/** Type for actual time in milliseconds. */
typedef uint64_t TTime;

/** Type for actual note states. */
struct TNoteState
{
    /** Indicates if the note is pressed (true if an on event was received last, false if an off event was received last). */
    bool pressed;
    /** Indicates if the note is sounding (either because it's pressed or the damper pedal is pressed). */
    bool sounding;
    /** The press down velocity. */
    uint8_t pressDownVelocity;
    /** The time stamp of the note on event. */
    TTime noteOnTimeStamp;
};
}


#endif /* PROCESSING_INTERFACES_PROCESSINGTYPES_H_ */
