/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Unit tests for ProcessingBlock.
 */

#include <algorithm>
#include <gtest/gtest.h>
#include <Drivers/Mock/MockMidiInput.h>

#include "../Mock/MockProcessingBlock.h"
#include "../ProcessingChain.h"
#include "../ProcessingBlockFactory.h"
#include "../Interfaces/ProcessingTypes.h"

using ::testing::_;
using ::testing::Invoke;

static void addRed(Processing::TRgbStrip& strip)
{
    for(auto& rLed : strip)
    {
        rLed.r = 10;
    }
}

static void addGreen(Processing::TRgbStrip& strip)
{
    for(auto& rLed : strip)
    {
        rLed.g = 10;
    }
}

static void doubleValue(Processing::TRgbStrip& strip)
{
    for(auto& rLed : strip)
    {
        rLed.r = std::min(0xff, rLed.r*2);
        rLed.g = std::min(0xff, rLed.g*2);
        rLed.b = std::min(0xff, rLed.b*2);
    }
}

class ProcessingChainTest
    : public ::testing::Test
{
public:
    static constexpr unsigned int c_stripSize = 3;

    ProcessingChainTest()
        : m_redSource()
        , m_greenSource()
        , m_valueDoubler()
        , m_strip(c_stripSize)
        , m_processingBlockFactory(m_mockMidiInput, m_noteToLightMap)
        , m_processingChain(m_processingBlockFactory)
    {
        ON_CALL(m_redSource, execute(_))
            .WillByDefault(Invoke(addRed));
        ON_CALL(m_greenSource, execute(_))
            .WillByDefault(Invoke(addGreen));
        ON_CALL(m_valueDoubler, execute(_))
            .WillByDefault(Invoke(doubleValue));
    }

    MockProcessingBlock m_redSource;
    MockProcessingBlock m_greenSource;
    MockProcessingBlock m_valueDoubler;

    Processing::TRgbStrip m_strip;

    MockMidiInput m_mockMidiInput;
    Processing::TNoteToLightMap m_noteToLightMap;
    ProcessingBlockFactory m_processingBlockFactory;
    ProcessingChain m_processingChain;
};

TEST_F(ProcessingChainTest, empty)
{
    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 1, 0, 0 };
    reference[1] = { 0, 1, 0 };
    reference[2] = { 0, 0, 1 };
    m_strip = reference;

    m_processingChain.execute(m_strip);
    EXPECT_EQ(reference, m_strip);
}

