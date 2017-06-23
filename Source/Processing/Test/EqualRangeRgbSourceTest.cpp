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
 * @brief <brief description of the file>
 */

#include <vector>
#include <string>
#include <gtest/gtest.h>
#include <json.hpp>

using json = nlohmann::json;

#include "../EqualRangeRgbSource.h"

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
