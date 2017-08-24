/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
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
 */

#include "gtest/gtest.h"
#include "Drivers/Mock/MockMidiInput.h"

#include "../Concert.h"
#include "../Mock/MockProcessingBlockFactory.h"

using testing::_;
using testing::SaveArg;
using testing::Return;

class ConcertTest
    : public testing::Test
{
public:
    ConcertTest()
        : m_mockProcessingBlockFactory()
        , m_mockMidiInput()
        , m_controlChangeCallback(0)
        , m_programChangeCallback(0)
    {
        // Store callbacks so we can simulate events
        EXPECT_CALL(m_mockMidiInput, subscribeControlChange(_))
            .WillOnce(DoAll(SaveArg<0>(&m_controlChangeCallback), Return(42)));
        EXPECT_CALL(m_mockMidiInput, subscribeProgramChange(_))
            .WillOnce(DoAll(SaveArg<0>(&m_programChangeCallback), Return(43)));
        
        m_pConcert = new Concert(m_mockMidiInput, m_mockProcessingBlockFactory);
    }

    void SetUp()
    {
        // It's not ideal to re-assert this in every test, but cleaner and safer as almost every test
        // will call these
        ASSERT_NE(nullptr, m_controlChangeCallback);
        ASSERT_NE(nullptr, m_programChangeCallback);
    }

    virtual ~ConcertTest()
    {
        EXPECT_CALL(m_mockMidiInput, unsubscribeControlChange(42));
        EXPECT_CALL(m_mockMidiInput, unsubscribeProgramChange(43));
        delete m_pConcert;
    }

    // Required mocks
    MockProcessingBlockFactory m_mockProcessingBlockFactory;
    MockMidiInput m_mockMidiInput;
    
    // Mock subscriptions
    IMidiInput::TControlChangeFunction m_controlChangeCallback;
    IMidiInput::TProgramChangeFunction m_programChangeCallback;

    // Object under test
    Concert*  m_pConcert;
};

TEST_F(ConcertTest, bankSelect)
{
    const uint8_t channel(0);
    const uint16_t bank(m_pConcert->getCurrentBank());
    
    m_pConcert->setListeningToProgramChange(true);
    m_pConcert->setProgramChangeChannel(channel);

    // Simulate a control change sequence
    m_controlChangeCallback(channel, IMidiInterface::BANK_SELECT_LSB, (bank + 1) & 0x0f);
    m_controlChangeCallback(channel, IMidiInterface::BANK_SELECT_MSB, (bank + 1) & 0xf0);

    m_pConcert->execute();

    // Check stored bank
    ASSERT_EQ(bank + 1, m_pConcert->getCurrentBank());
}

TEST_F(ConcertTest, bankSelectFromOtherChannelIgnored)
{
    const uint8_t channel(0);
    const uint16_t bank(m_pConcert->getCurrentBank());
    
    m_pConcert->setListeningToProgramChange(true);
    m_pConcert->setProgramChangeChannel(channel);

    // Simulate a control change sequence
    m_controlChangeCallback(channel + 1, IMidiInterface::BANK_SELECT_LSB, (bank + 1) & 0x0f);
    m_controlChangeCallback(channel + 1, IMidiInterface::BANK_SELECT_MSB, (bank + 1) & 0xf0);

    m_pConcert->execute();

    // Check stored bank
    ASSERT_EQ(bank, m_pConcert->getCurrentBank());
}
