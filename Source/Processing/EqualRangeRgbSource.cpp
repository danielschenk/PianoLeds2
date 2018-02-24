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

#include <Common/Utilities/JsonHelper.h>

#include "EqualRangeRgbSource.h"

EqualRangeRgbSource::EqualRangeRgbSource()
    : m_mutex()
    , m_color()
{
}

EqualRangeRgbSource::~EqualRangeRgbSource()
{
}

void EqualRangeRgbSource::activate()
{
}

void EqualRangeRgbSource::deactivate()
{
}

void EqualRangeRgbSource::execute(Processing::TRgbStrip& strip)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for(auto& it : strip)
    {
        it.r = m_color.r;
        it.g = m_color.g;
        it.b = m_color.b;
    }
}

Processing::TRgb EqualRangeRgbSource::getColor() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_color;
}

void EqualRangeRgbSource::setColor(Processing::TRgb color)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_color = color;
}

json EqualRangeRgbSource::convertToJson() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    json json;
    json[IJsonConvertible::c_objectTypeKey] = std::string(IProcessingBlock::c_typeNameEqualRangeRgbSource);
    json[c_rJsonKey] = m_color.r;
    json[c_gJsonKey] = m_color.g;
    json[c_bJsonKey] = m_color.b;

    return json;
}

void EqualRangeRgbSource::convertFromJson(json json)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    JsonHelper helper(__PRETTY_FUNCTION__, json);
    helper.getItemIfPresent(c_rJsonKey, m_color.r);
    helper.getItemIfPresent(c_gJsonKey, m_color.g);
    helper.getItemIfPresent(c_bJsonKey, m_color.b);
}
