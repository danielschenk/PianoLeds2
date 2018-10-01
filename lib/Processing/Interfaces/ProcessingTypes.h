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
 * @brief Processing type definitions.
 */

#ifndef PROCESSING_INTERFACES_PROCESSINGTYPES_H_
#define PROCESSING_INTERFACES_PROCESSINGTYPES_H_

#include <cstdint>
#include <map>
#include <vector>
#include <string>
#include <json11.hpp>
using Json = json11::Json;

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
    bool operator==(const TRgb& other) const;
    bool operator!=(const TRgb& other) const;

    /**
     * Multiply every color by a single factor.
     */
    TRgb operator*(float factor) const;

    /**
     * Add colors together.
     */
    TRgb operator+(const TRgb& other) const;

    /**
     * Subtract colors from each other.
     */
    TRgb operator-(const TRgb& other) const;

    /**
     * Assign colors.
     */
    TRgb& operator=(const TRgb& other) = default;

    /**
     * Add colors together and assign the result to this object.
     */
    TRgb& operator+=(const TRgb& other);

    /**
     * Subtract colors from each other and assign the result to this object.
     */
    TRgb& operator-=(const TRgb& other);

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
typedef std::map<uint8_t, uint16_t> TNoteToLightMap;

Json convert(const TNoteToLightMap& source);
TNoteToLightMap convert(const Json& source);

/** Type for actual time in milliseconds. */
typedef uint32_t TTime;

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
