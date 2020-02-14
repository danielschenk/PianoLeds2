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

#include "EqualRangeRgbSource.h"
#include "Json11Helper.h"

void EqualRangeRgbSource::activate()
{
}

void EqualRangeRgbSource::deactivate()
{
}

void EqualRangeRgbSource::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    std::lock_guard<std::mutex> lock(mutex);

    for(auto& it : strip)
    {
        it.r = color.r;
        it.g = color.g;
        it.b = color.b;
    }
}

Processing::TRgb EqualRangeRgbSource::getColor() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return color;
}

void EqualRangeRgbSource::setColor(Processing::TRgb color)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->color = color;
}

Json EqualRangeRgbSource::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object json;
    json[IJsonConvertible::c_objectTypeKey] = getObjectType();
    json[c_rJsonKey] = color.r;
    json[c_gJsonKey] = color.g;
    json[c_bJsonKey] = color.b;

    return Json(json);
}

void EqualRangeRgbSource::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(c_rJsonKey, color.r);
    helper.getItemIfPresent(c_gJsonKey, color.g);
    helper.getItemIfPresent(c_bJsonKey, color.b);
}

std::string EqualRangeRgbSource::getObjectType() const
{
     return IProcessingBlock::c_typeNameEqualRangeRgbSource;
}
