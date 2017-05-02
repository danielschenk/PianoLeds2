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
using ::testing::NiceMock;

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

    // Make processing block mocks nice, to prevent warnings about unexpected calls to execute().
    typedef NiceMock<MockProcessingBlock> TMockBlock;

    ProcessingChainTest()
        : m_pRedSource(new TMockBlock())
        , m_pGreenSource(new TMockBlock())
        , m_pValueDoubler(new TMockBlock())
        , m_strip(c_stripSize)
        , m_mockMidiInput()
        , m_noteToLightMap()
        , m_processingBlockFactory(m_mockMidiInput, m_noteToLightMap)
        , m_processingChain(m_processingBlockFactory)
    {
        ON_CALL(*m_pRedSource, execute(_))
            .WillByDefault(Invoke(addRed));
        ON_CALL(*m_pGreenSource, execute(_))
            .WillByDefault(Invoke(addGreen));
        ON_CALL(*m_pValueDoubler, execute(_))
            .WillByDefault(Invoke(doubleValue));
    }

    virtual ~ProcessingChainTest()
    {
        delete m_pRedSource;
        m_pRedSource = nullptr;
        delete m_pGreenSource;
        m_pGreenSource = nullptr;
        delete m_pValueDoubler;
        m_pValueDoubler = nullptr;
    }

    // These have to be pointers, as the processing chain takes ownership and will try to delete it's children.
    TMockBlock* m_pRedSource;
    TMockBlock* m_pGreenSource;
    TMockBlock* m_pValueDoubler;

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

TEST_F(ProcessingChainTest, insertOne)
{
    m_processingChain.insertBlock(m_pRedSource);
    m_pRedSource = nullptr;

    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 10, 0, 0 };
    reference[1] = { 10, 0, 0 };
    reference[2] = { 10, 0, 0 };

    m_processingChain.execute(m_strip);
    EXPECT_EQ(reference, m_strip);
}

TEST_F(ProcessingChainTest, insertTwo)
{
    m_processingChain.insertBlock(m_pRedSource);
    m_pRedSource = nullptr;
    m_processingChain.insertBlock(m_pValueDoubler);
    m_pValueDoubler = nullptr;

    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 20, 0, 0 };
    reference[1] = { 20, 0, 0 };
    reference[2] = { 20, 0, 0 };

    m_processingChain.execute(m_strip);
    EXPECT_EQ(reference, m_strip);
}
