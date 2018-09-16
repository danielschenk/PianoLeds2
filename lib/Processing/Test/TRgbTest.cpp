/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2018 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 */

#include <gtest/gtest.h>

#include "../ProcessingTypes.h"

using namespace Processing;

TEST(TRgbTest, defaultConstructor)
{
    TRgb color;
    EXPECT_EQ(0, color.r);
    EXPECT_EQ(0, color.g);
    EXPECT_EQ(0, color.b);
}

TEST(TRgbTest, constructor)
{
    TRgb color(1, 2, 3);
    EXPECT_EQ(1, color.r);
    EXPECT_EQ(2, color.g);
    EXPECT_EQ(3, color.b);
}

TEST(TRgbTest, addition)
{
    TRgb color1(1, 2, 3), color2(10, 20, 30);
    TRgb result(color1 + color2);

    EXPECT_EQ(11, result.r);
    EXPECT_EQ(22, result.g);
    EXPECT_EQ(33, result.b);
}

TEST(TRgbTest, additionWithClipping)
{
    TRgb color1(250, 250, 250), color2(1, 20, 30);
    TRgb result(color1 + color2);

    EXPECT_EQ(251, result.r);
    EXPECT_EQ(255, result.g);
    EXPECT_EQ(255, result.b);
}

TEST(TRgbTest, subtraction)
{
    TRgb color1(11, 22, 33), color2(1, 2, 3);
    TRgb result(color1 - color2);

    EXPECT_EQ(10, result.r);
    EXPECT_EQ(20, result.g);
    EXPECT_EQ(30, result.b);
}

TEST(TRgbTest, subtractionWithClipping)
{
    TRgb color1(5, 5, 5), color2(1, 20, 30);
    TRgb result(color1 - color2);

    EXPECT_EQ(4, result.r);
    EXPECT_EQ(0, result.g);
    EXPECT_EQ(0, result.b);
}

TEST(TRgbTest, multiplication)
{
    TRgb color(10, 20, 30);
    TRgb result(color * 1.5);

    EXPECT_EQ(15, result.r);
    EXPECT_EQ(30, result.g);
    EXPECT_EQ(45, result.b);
}

TEST(TRgbTest, multiplicationWithClipping)
{
    TRgb color(10, 20, 30);
    TRgb result(color * 20);

    EXPECT_EQ(200, result.r);
    EXPECT_EQ(255, result.g);
    EXPECT_EQ(255, result.b);
}
