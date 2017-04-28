/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Unit test for NoteRgbSource.
 */

#include <gtest/gtest.h>
#include <Drivers/Mock/MockMidiInput.h>

#include "../NoteRgbSource.h"
#include "../LinearRgbFunction.h"

using ::testing::_;
using ::testing::SaveArg;
using ::testing::Return;

class NoteRgbSourceTest
    : public ::testing::Test
{
public:
    static constexpr unsigned int c_StripSize = 10;

    NoteRgbSourceTest()
        : m_mockMidiInput()
        , m_strip(c_StripSize)
    {
        // Store callbacks so we can simulate events
        EXPECT_CALL(m_mockMidiInput, subscribeNoteOnOff(_))
            .WillOnce(DoAll(SaveArg<0>(&m_noteOnOffCallback), Return(42)));
        EXPECT_CALL(m_mockMidiInput, subscribeControlChange(_))
            .WillOnce(DoAll(SaveArg<0>(&m_controlChangeCallback), Return(69)));

        for(int i = 0; i < c_StripSize; ++i)
        {
            // Default: simple 1-to-1 mapping
            m_noteToLightMap[i] = i;
        }
        m_pNoteRgbSource = new NoteRgbSource(m_mockMidiInput, m_noteToLightMap);
    }

    void SetUp()
    {
        // It's not ideal to re-assert this in every test, but cleaner and safer as almost every test
        // will call these
        ASSERT_NE(nullptr, m_noteOnOffCallback);
        ASSERT_NE(nullptr, m_controlChangeCallback);
    }

    virtual ~NoteRgbSourceTest()
    {
        EXPECT_CALL(m_mockMidiInput, unsubscribeNoteOnOff(42));
        EXPECT_CALL(m_mockMidiInput, unsubscribeControlChange(69));
        delete m_pNoteRgbSource;
    }

    MockMidiInput m_mockMidiInput;
    NoteRgbSource* m_pNoteRgbSource;
    Processing::TRgbStrip m_strip;

    IMidiInput::TNoteOnOffFunction m_noteOnOffCallback;
    IMidiInput::TControlChangeFunction m_controlChangeCallback;

    Processing::TNoteToLightMap m_noteToLightMap;
};

TEST_F(NoteRgbSourceTest, noNotesSounding)
{
    // m_strip is initialized with zeroes
    auto darkStrip = m_strip;

    m_strip[0] = {4, 5, 6};
    m_strip[6] = {7, 8, 9};
    m_strip[c_StripSize-1] = {11, 12, 13};

    m_pNoteRgbSource->execute(m_strip);

    // No notes sounding should cause darkness
    ASSERT_EQ(darkStrip, m_strip);
}

TEST_F(NoteRgbSourceTest, noteOn)
{
    // (channel, number, velocity, on/off)
    m_noteOnOffCallback(0, 0, 1, true);
    m_noteOnOffCallback(0, 5, 6, true);

    m_pNoteRgbSource->execute(m_strip);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto reference = Processing::TRgbStrip(c_StripSize);
    reference[0] = {0xff, 0xff, 0xff};
    reference[5] = {0xff, 0xff, 0xff};

    EXPECT_EQ(reference, m_strip);
}

TEST_F(NoteRgbSourceTest, noteOff)
{
    // (channel, number, velocity, on/off)
    m_noteOnOffCallback(0, 0, 1, true);
    m_noteOnOffCallback(0, 5, 6, true);

    m_noteOnOffCallback(0, 0, 8, false);

    m_pNoteRgbSource->execute(m_strip);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto reference = Processing::TRgbStrip(c_StripSize);
    reference[5] = {0xff, 0xff, 0xff};

    EXPECT_EQ(reference, m_strip);
}

TEST_F(NoteRgbSourceTest, ignoreOtherChannel)
{
    m_noteOnOffCallback(1, 0, 1, true);

    auto darkStrip = m_strip;
    m_pNoteRgbSource->execute(m_strip);
    EXPECT_EQ(darkStrip, m_strip);
}

TEST_F(NoteRgbSourceTest, ignorePedal)
{
    auto darkStrip = m_strip;

    m_noteOnOffCallback(0, 0, 1, true);
    m_controlChangeCallback(0, IMidiInterface::DAMPER_PEDAL, 0xff);
    m_noteOnOffCallback(0, 0, 1, false);

    m_pNoteRgbSource->execute(m_strip);
    EXPECT_EQ(darkStrip, m_strip);
}

TEST_F(NoteRgbSourceTest, usePedal)
{
    m_pNoteRgbSource->setUsingPedal(true);

    // Press a key
    // (channel, number, velocity, on/off)
    m_noteOnOffCallback(0, 0, 1, true);
    // Press pedal
    // (channel, number, value)
    m_controlChangeCallback(0, IMidiInterface::DAMPER_PEDAL, 0xff);
    // Press another key
    m_noteOnOffCallback(0, 2, 1, true);

    auto reference = Processing::TRgbStrip(c_StripSize);
    // Both notes are still sounding
    reference[0] = {0xff, 0xff, 0xff};
    reference[2] = {0xff, 0xff, 0xff};
    m_pNoteRgbSource->execute(m_strip);
    EXPECT_EQ(reference, m_strip);

    // Release keys
    m_noteOnOffCallback(0, 0, 1, false);
    m_noteOnOffCallback(0, 2, 1, false);

    // Both notes are still sounding
    m_pNoteRgbSource->execute(m_strip);
    EXPECT_EQ(reference, m_strip);

    // Release pedal
    m_controlChangeCallback(0, IMidiInterface::DAMPER_PEDAL, 0);

    // Notes are not sounding anymore
    reference[0] = {0, 0, 0};
    reference[2] = {0, 0, 0};
    m_pNoteRgbSource->execute(m_strip);
    EXPECT_EQ(reference, m_strip);
}

TEST_F(NoteRgbSourceTest, otherRgbFunction)
{
    // LinearRgbFunction takes a factor&offset pair for each color
    m_pNoteRgbSource->setRgbFunction(new LinearRgbFunction({0, 0}, {10, 0}, {1, 10}));

    // (channel, number, velocity, on/off)
    m_noteOnOffCallback(0, 0, 1, true);
    m_noteOnOffCallback(0, 5, 6, true);

    m_pNoteRgbSource->execute(m_strip);

    auto reference = Processing::TRgbStrip(c_StripSize);
    reference[0] = {0, 10, 11};
    reference[5] = {0, 60, 16};

    EXPECT_EQ(reference, m_strip);
}

TEST_F(NoteRgbSourceTest, otherNoteToLightMap)
{
    m_noteToLightMap.clear();
    m_noteToLightMap[0] = 9;
    m_noteToLightMap[5] = 8;

    // (channel, number, velocity, on/off)
    m_noteOnOffCallback(0, 0, 1, true);
    m_noteOnOffCallback(0, 5, 6, true);

    m_pNoteRgbSource->execute(m_strip);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto reference = Processing::TRgbStrip(c_StripSize);
    reference[9] = {0xff, 0xff, 0xff};
    reference[8] = {0xff, 0xff, 0xff};

    EXPECT_EQ(reference, m_strip);
}

TEST_F(NoteRgbSourceTest, doNotWriteOutsideStrip)
{
    // (channel, number, velocity, on/off)
    m_noteOnOffCallback(0, 0, 1, true);
    m_noteOnOffCallback(0, 9, 6, true);

    auto shorterStrip = Processing::TRgbStrip(5);
    m_pNoteRgbSource->execute(shorterStrip);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto reference = Processing::TRgbStrip(5);
    reference[0] = {0xff, 0xff, 0xff};

    EXPECT_EQ(reference, shorterStrip);
}
