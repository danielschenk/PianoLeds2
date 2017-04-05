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

void EqualRangeRgbSource::execute(Processing::TRgbStrip& output)
{
    for(auto& it : output)
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
