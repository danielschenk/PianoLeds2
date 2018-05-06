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

#include "../EqualRangeRgbSource.h"
using json = nlohmann::json;

class EqualRangeRgbSourceTest
    : public ::testing::Test
{
public:
    EqualRangeRgbSource m_source;
};

TEST_F(EqualRangeRgbSourceTest, executeDifferentColors)
{
    Processing::TRgbStrip strip(20);
    std::vector<Processing::TRgb> colors({{0, 0, 0}, {255, 255, 255}, {1, 2, 3}});

    for(const auto& colorIt : colors)
    {
        m_source.setColor(colorIt);
        m_source.execute(strip);
        for(const auto& outputIt : strip)
        {
            EXPECT_EQ(outputIt, colorIt);
        }
    }
}

TEST_F(EqualRangeRgbSourceTest, convertFromJson)
{
    json j = R"(
        {
            "objectType": "EqualRangeRgbSource",
            "r": 10,
            "g": 20,
            "b": 30
        }
    )"_json;

    m_source.convertFromJson(j);
    EXPECT_EQ(m_source.getColor(), Processing::TRgb({10, 20, 30}));
}

TEST_F(EqualRangeRgbSourceTest, convertFromJsonWithWrongType)
{
    json j = R"(
        {
            "objectType": "wrong",
            "r": 10,
            "g": 20,
            "b": 30
        }
    )"_json;

    m_source.convertFromJson(j);
    EXPECT_EQ(m_source.getColor(), Processing::TRgb({10, 20, 30}));
}

TEST_F(EqualRangeRgbSourceTest, convertFromJsonWithMissingColor)
{
    json j = R"(
        {
            "objectType": "EqualRangeRgbSource",
            "r": 10,
            "b": 30
        }
    )"_json;

    m_source.convertFromJson(j);
    EXPECT_EQ(m_source.getColor(), Processing::TRgb({10, 0, 30}));
}

TEST_F(EqualRangeRgbSourceTest, convertToJson)
{
    m_source.setColor(Processing::TRgb({40, 50, 60}));

    json j = m_source.convertToJson();
    EXPECT_EQ(4, j.size());
    EXPECT_EQ("EqualRangeRgbSource", j.at("objectType").get<std::string>());
    EXPECT_EQ(40, j.at("r").get<int>());
    EXPECT_EQ(50, j.at("g").get<int>());
    EXPECT_EQ(60, j.at("b").get<int>());
}
