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
#include <vector>

namespace Processing
{

/** Type for single RGB color. */
struct TRgb
{
    /**
     * Constructor.
     *
     * @param[in]   initialR    Initial red value.
     * @param[in]   initialG    Initial green value.
     * @param[in]   initialB    Initial blue value.
     */
    TRgb(uint8_t initialR, uint8_t initialG, uint8_t initialB);

    /**
     * Default constructor, initializes values to 0.
     */
    TRgb();

    /**
     * Compare with another @ref TRgb.
     */
    bool operator ==(const TRgb& other) const;

    /**
     * Multiply every color by a single factor.
     */
    TRgb operator *(float factor) const;

    uint8_t r;
    uint8_t g;
    uint8_t b;
};

/**
 * Multiply every color by a single factor.
 */
TRgb operator*(float factor, const TRgb& color);

/**
 * Construct color from float values, clamping if necessary.
 */
TRgb rgbFromFloat(float initialR, float initialG, float initialB);

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

} /* namespace Processing */


#endif /* PROCESSING_INTERFACES_PROCESSINGTYPES_H_ */
