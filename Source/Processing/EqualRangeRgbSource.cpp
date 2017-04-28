/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 */

#include <Processing/EqualRangeRgbSource.h>

EqualRangeRgbSource::EqualRangeRgbSource()
    : m_color()
{
}

EqualRangeRgbSource::~EqualRangeRgbSource()
{
}

void EqualRangeRgbSource::execute(Processing::TRgbStrip& strip)
{
    for(auto& it : strip)
    {
        it.r = m_color.r;
        it.g = m_color.g;
        it.b = m_color.b;
    }
}

Processing::TRgb EqualRangeRgbSource::getColor() const
{
    return m_color;
}

void EqualRangeRgbSource::setColor(Processing::TRgb color)
{
    m_color = color;
}

json EqualRangeRgbSource::convertToJson() const
{
    json json;
    json[IJsonConvertible::c_objectTypeKey] = std::string(IProcessingBlock::c_typeNameEqualRangeRgbSource);
    json[c_rJsonKey] = m_color.r;
    json[c_gJsonKey] = m_color.g;
    json[c_bJsonKey] = m_color.b;

    return json;
}

void EqualRangeRgbSource::convertFromJson(json json)
{
    if(json.count(c_rJsonKey) > 0)
    {
        m_color.r = json[c_rJsonKey];
    }
    if(json.count(c_gJsonKey) > 0)
    {
        m_color.g = json[c_gJsonKey];
    }
    if(json.count(c_bJsonKey) > 0)
    {
        m_color.b = json[c_bJsonKey];
    }
}
