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
 */

#include <Common/Utilities/JsonHelper.h>

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

json LinearRgbFunction::convertToJson() const
{
    json json;
    json[IJsonConvertible::c_objectTypeKey] = std::string(IRgbFunction::c_jsonTypeNameLinearRgbFunction);
    json[c_rFactorJsonKey] = m_redConstants.factor;
    json[c_gFactorJsonKey] = m_greenConstants.factor;
    json[c_bFactorJsonKey] = m_blueConstants.factor;
    json[c_rOffsetJsonKey] = m_redConstants.offset;
    json[c_gOffsetJsonKey] = m_greenConstants.offset;
    json[c_bOffsetJsonKey] = m_blueConstants.offset;

    return json;
}

void LinearRgbFunction::convertFromJson(json json)
{
    JsonHelper helper(__PRETTY_FUNCTION__, json);
    helper.getItemIfPresent(c_rFactorJsonKey, m_redConstants.factor);
    helper.getItemIfPresent(c_rOffsetJsonKey, m_redConstants.offset);
    helper.getItemIfPresent(c_gFactorJsonKey, m_greenConstants.factor);
    helper.getItemIfPresent(c_gOffsetJsonKey, m_greenConstants.offset);
    helper.getItemIfPresent(c_bFactorJsonKey, m_blueConstants.factor);
    helper.getItemIfPresent(c_bOffsetJsonKey, m_blueConstants.offset);
}
