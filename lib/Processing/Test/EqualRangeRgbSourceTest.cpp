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
 * @brief <brief description of the file>
 */

#include <vector>
#include <string>
#include <gtest/gtest.h>

#include "Json11Helper.h"
#include "../EqualRangeRgbSource.h"
#include "Mock/MockTime.h"
#include "LoggingEntryPoint.h"

using ::testing::NiceMock;

class EqualRangeRgbSourceTest
    : public ::testing::Test
{
public:
    EqualRangeRgbSourceTest()
        : m_time()
        , m_source()
    {
        LoggingEntryPoint::setTime(&m_time);
    }

    NiceMock<MockTime> m_time;
    EqualRangeRgbSource m_source;
};

TEST_F(EqualRangeRgbSourceTest, executeDifferentColors)
{
    Processing::TRgbStrip strip(20);
    std::vector<Processing::TRgb> colors({{0, 0, 0}, {255, 255, 255}, {1, 2, 3}});

    for(const auto& colorIt : colors)
    {
        m_source.setColor(colorIt);
        m_source.execute(strip, Processing::TNoteToLightMap());
        for(const auto& outputIt : strip)
        {
            EXPECT_EQ(outputIt, colorIt);
        }
    }

}
TEST_F(EqualRangeRgbSourceTest, convertFromJson)
{
    std::string err;
    Json j = Json::parse(R"(
        {
            "objectType": "EqualRangeRgbSource",
            "r": 10,
            "g": 20,
            "b": 30
        }
        )",
        err,
        json11::STANDARD);

    m_source.convertFromJson(j);
    EXPECT_EQ(m_source.getColor(), Processing::TRgb({10, 20, 30}));
}

TEST_F(EqualRangeRgbSourceTest, convertFromJsonWithWrongType)
{
    std::string err;
    Json j = Json::parse(R"(
        {
            "objectType": "wrong",
            "r": 10,
            "g": 20,
            "b": 30
        }
        )",
        err,
        json11::STANDARD);

    m_source.convertFromJson(j);
    EXPECT_EQ(m_source.getColor(), Processing::TRgb({10, 20, 30}));
}

TEST_F(EqualRangeRgbSourceTest, convertFromJsonWithMissingColor)
{
    std::string err;
    Json j = Json::parse(R"(
        {
            "objectType": "EqualRangeRgbSource",
            "r": 10,
            "b": 30
        }
        )",
        err,
        json11::STANDARD);

    m_source.convertFromJson(j);
    EXPECT_EQ(m_source.getColor(), Processing::TRgb({10, 0, 30}));
}

TEST_F(EqualRangeRgbSourceTest, convertToJson)
{
    m_source.setColor(Processing::TRgb({40, 50, 60}));

    Json::object j = m_source.convertToJson().object_items();
    EXPECT_EQ(4, j.size());
    EXPECT_EQ("EqualRangeRgbSource", j.at("objectType").string_value());
    EXPECT_EQ(40, j.at("r").number_value());
    EXPECT_EQ(50, j.at("g").number_value());
    EXPECT_EQ(60, j.at("b").number_value());
}
