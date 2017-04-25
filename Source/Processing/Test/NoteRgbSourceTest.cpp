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

using ::testing::_;
using ::testing::SaveArg;
using ::testing::Return;

class NoteRgbSourceTest
    : public ::testing::Test
{
public:
    static constexpr unsigned int c_StripSize = 50;

    NoteRgbSourceTest()
        : m_mockMidiInput()
        , m_strip(c_StripSize)
    {
        // Store callbacks so we can simulate events
        EXPECT_CALL(m_mockMidiInput, subscribeNoteOnOff(_))
            .WillOnce(DoAll(SaveArg<0>(&m_noteOnOffCallback), Return(42)));
        EXPECT_CALL(m_mockMidiInput, subscribeControlChange(_))
            .WillOnce(DoAll(SaveArg<0>(&m_controlChangeCallback), Return(69)));
        m_pNoteRgbSource = new NoteRgbSource(m_mockMidiInput);

        for(int i = 0; i < c_StripSize; ++i)
        {
            // Default: simple 1-to-1 mapping
            m_noteToLightMap[i] = i;
        }
        m_pNoteRgbSource->setNoteToLightMap(m_noteToLightMap);
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
    m_noteOnOffCallback(0, 42, 1, true);
    m_noteOnOffCallback(0, 43, 6, true);

    m_pNoteRgbSource->execute(m_strip);

    // Default: white, factor 255, so any velocity >0 will cause full on
    for(int i = 0; i < c_StripSize; ++i)
    {
        Processing::TRgb color;
        if(i == 42 || i == 43)
        {
            color = {255, 255, 255};
        }
        EXPECT_EQ(color, m_strip[i]) << "where i is" << i;
    }
}

TEST_F(NoteRgbSourceTest, noteOff)
{
    // (channel, number, velocity, on/off)
    m_noteOnOffCallback(0, 42, 1, true);
    m_noteOnOffCallback(0, 43, 6, true);

    m_noteOnOffCallback(0, 42, 8, false);

    m_pNoteRgbSource->execute(m_strip);

    // Default: white, factor 255, so any velocity >0 will cause full on
    for(int i = 0; i < c_StripSize; ++i)
    {
        Processing::TRgb color;
        if(i == 43)
        {
            color = {255, 255, 255};
        }
        EXPECT_EQ(color, m_strip[i]) << "where i is " << i;
    }
}
