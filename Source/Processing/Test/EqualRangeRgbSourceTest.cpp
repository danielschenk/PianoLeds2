/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief <brief description of the file>
 */

#include <gtest/gtest.h>
#include <vector>

#include "../EqualRangeRgbSource.h"

class EqualRangeRgbSourceTest
    : public ::testing::Test
{
public:
    EqualRangeRgbSource m_source;
};

TEST_F(EqualRangeRgbSourceTest, ExecuteDifferentColors)
{
    Processing::TRgbStrip strip(20);
    std::vector<Processing::TRgb> colors({{0, 0, 0}, {255, 255, 255}, {1, 2, 3}});

    for(const auto& colorIt : colors)
    {
        m_source.setColor(colorIt);
        m_source.execute(strip);
        for(const auto& outputIt : strip)
        {
            ASSERT_EQ(outputIt, colorIt);
        }
    }
}
