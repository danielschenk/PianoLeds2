/**
 * @file
 *
 * MIT License
 *
 * @copyright (c) Daniel Schenk, 2019
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

#include "PianoDecayRgbFunction.h"
#include "gtest/gtest.h"

#include <vector>

class PianoDecayRgbFunctionTest : public testing::TestWithParam<uint8_t>
{
public:
    struct TTruthTableEntry
    {
        Processing::TTime time;
        Processing::TRgb result;
    };

    PianoDecayRgbFunctionTest()
        : m_function()
    {
    }

    ~PianoDecayRgbFunctionTest() override = default;

    // Unit under test
    PianoDecayRgbFunction m_function;
};

INSTANTIATE_TEST_CASE_P(Velocities,
                        PianoDecayRgbFunctionTest,
                        testing::Range(static_cast<uint8_t>(0), static_cast<uint8_t>(127),
                                static_cast<uint8_t>(10)));

TEST_P(PianoDecayRgbFunctionTest, decay)
{
    std::vector<TTruthTableEntry> truthTable = {
            {0, {200, 100, 100}},
            {600, {150, 75, 75}},
            {1200, {100, 50, 50}},
            {8100, {50, 25, 25}},
            {15000, {0, 0, 0}},
    };

    const Processing::TNoteState noteState = {
            .noteOnTimeStamp = 0,
            .pressDownVelocity = GetParam(),
            .pressed = true,
            .sounding = true,
    };
    const float velocityFactor(static_cast<float>(noteState.pressDownVelocity) / 127.0f);

    const Processing::TRgb color = {200, 100, 100};
    m_function.setColor(color);

    for(const auto& entry : truthTable)
    {
        ASSERT_EQ(entry.result * velocityFactor, m_function.calculate(noteState, entry.time))
            << "(time " << entry.time << ")";
    }
}

TEST_F(PianoDecayRgbFunctionTest, notSounding)
{
    const Processing::TNoteState noteState = {
            .noteOnTimeStamp = 0,
            .pressDownVelocity = 127,
            .pressed = false,
            .sounding = false,
    };

    m_function.setColor(Processing::TRgb(255, 255, 255));
    ASSERT_EQ(Processing::TRgb(0, 0, 0), m_function.calculate(noteState, 42));
}
