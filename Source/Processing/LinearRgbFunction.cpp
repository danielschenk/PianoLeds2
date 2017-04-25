/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 */

#include "LinearRgbFunction.h"

LinearRgbFunction::LinearRgbFunction(TLinearConstants redConstants, TLinearConstants greenConstants, TLinearConstants blueConstants)
    : m_redConstants(redConstants)
    , m_greenConstants(greenConstants)
    , m_blueConstants(blueConstants)
{
}

LinearRgbFunction::~LinearRgbFunction()
{
}

Processing::TRgb LinearRgbFunction::calculate(const Processing::TNoteState& noteState, Processing::TTime currentTime) const
{
    Processing::TRgb output;

    if(noteState.sounding)
    {
        output = Processing::rgbFromFloat(
            m_redConstants.factor * noteState.pressDownVelocity + m_redConstants.offset,
            m_greenConstants.factor * noteState.pressDownVelocity + m_greenConstants.offset,
            m_blueConstants.factor * noteState.pressDownVelocity + m_blueConstants.offset
        );
    }

    return output;
}
