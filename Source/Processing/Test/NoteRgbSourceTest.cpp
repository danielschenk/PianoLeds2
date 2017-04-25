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
    NoteRgbSourceTest()
        : m_mockMidiInput()
        , m_strip(50)
    {
        // Store callbacks so we can simulate events
        EXPECT_CALL(m_mockMidiInput, subscribeNoteOnOff(_))
            .WillOnce(DoAll(SaveArg<0>(&m_noteOnOffCallback), Return(42)));
        EXPECT_CALL(m_mockMidiInput, subscribeControlChange(_))
            .WillOnce(DoAll(SaveArg<0>(&m_controlChangeCallback), Return(69)));
        m_pNoteRgbSource = new NoteRgbSource(m_mockMidiInput);

        for(int i = 0; i < m_strip.size(); ++i)
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

TEST_F(NoteRgbSourceTest, noEventsNoChange)
{
    Processing::TNoteToLightMap noteToLightMap;

    for(int i = 0; i < m_strip.size(); ++i)
    {
        m_strip[i] = {12, 23, 34};
    }
    auto reference = m_strip;

    m_pNoteRgbSource->execute(m_strip);
    ASSERT_EQ(reference, m_strip);
}

