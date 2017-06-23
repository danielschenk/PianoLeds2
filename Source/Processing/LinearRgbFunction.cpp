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
    if(json.count(c_rFactorJsonKey) > 0)
    {
        m_redConstants.factor = json[c_rFactorJsonKey];
    }
    if(json.count(c_gFactorJsonKey) > 0)
    {
        m_greenConstants.factor = json[c_gFactorJsonKey];
    }
    if(json.count(c_bFactorJsonKey) > 0)
    {
        m_blueConstants.factor = json[c_bFactorJsonKey];
    }
    if(json.count(c_rOffsetJsonKey) > 0)
    {
        m_redConstants.offset = json[c_rOffsetJsonKey];
    }
    if(json.count(c_gOffsetJsonKey) > 0)
    {
        m_greenConstants.offset = json[c_gOffsetJsonKey];
    }
    if(json.count(c_bOffsetJsonKey) > 0)
    {
        m_blueConstants.offset = json[c_bOffsetJsonKey];
    }
}
