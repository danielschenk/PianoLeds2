/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Function which describes a time invariant linear relation between note state and RGB output.
 */

#ifndef PROCESSING_LINEARRGBFUNCTION_H_
#define PROCESSING_LINEARRGBFUNCTION_H_

#include "Interfaces/IRgbFunction.h"

/**
 * Function which describes a time invariant linear relation between note state and RGB output.
 */
class LinearRgbFunction
    : public IRgbFunction
{
public:
    struct TLinearConstants
    {
        float factor;
        float offset;
    };

    /**
     * Constructor.
     */
    LinearRgbFunction() = default;

    /**
     * Constructor.
     */
    LinearRgbFunction(TLinearConstants redConstants, TLinearConstants greenConstants, TLinearConstants blueConstants);

    /**
     * Destructor.
     */
    virtual ~LinearRgbFunction();

    // IRgbFunction implementation
    virtual Processing::TRgb calculate(const Processing::TNoteState& noteState, Processing::TTime currentTime) const;
    virtual json convertToJson() const;
    virtual void convertFromJson(json json);

private:
    TLinearConstants m_redConstants;
    TLinearConstants m_greenConstants;
    TLinearConstants m_blueConstants;

    static constexpr const char* c_rFactorJsonKey = "rFactor";
    static constexpr const char* c_gFactorJsonKey = "gFactor";
    static constexpr const char* c_bFactorJsonKey = "bFactor";
    static constexpr const char* c_rOffsetJsonKey = "rOffset";
    static constexpr const char* c_gOffsetJsonKey = "gOffset";
    static constexpr const char* c_bOffsetJsonKey = "bOffset";
};

#endif /* PROCESSING_LINEARRGBFUNCTION_H_ */
