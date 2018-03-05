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
#include "../Mock/MockPatch.h"

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

    // Should not be default and go beyond the byte range, to test LSB and MSB
    static const uint16_t c_testBankNumber;

    // Required mocks
    MockProcessingBlockFactory m_mockProcessingBlockFactory;
    MockMidiInput m_mockMidiInput;
    
    // Mock subscriptions
    IMidiInput::TControlChangeFunction m_controlChangeCallback;
    IMidiInput::TProgramChangeFunction m_programChangeCallback;

    // Object under test
    Concert*  m_pConcert;
};

const uint16_t ConcertTest::c_testBankNumber = 300;

TEST_F(ConcertTest, bankSelect)
{
    const uint8_t channel(0);
    
    m_pConcert->setListeningToProgramChange(true);
    m_pConcert->setProgramChangeChannel(channel);

    // Simulate a bank select sequence
    m_controlChangeCallback(channel, IMidiInterface::BANK_SELECT_LSB, c_testBankNumber & 0xff);
    m_controlChangeCallback(channel, IMidiInterface::BANK_SELECT_MSB, c_testBankNumber >> 8);

    m_pConcert->execute();

    // Check stored bank
    ASSERT_EQ(c_testBankNumber, m_pConcert->getCurrentBank());
}

TEST_F(ConcertTest, bankSelectFromOtherChannelIgnored)
{
    const uint8_t channel(0);
    const uint16_t bank(m_pConcert->getCurrentBank());
    
    m_pConcert->setListeningToProgramChange(true);
    m_pConcert->setProgramChangeChannel(channel);

    // Simulate a bank select sequence
    m_controlChangeCallback(channel + 1, IMidiInterface::BANK_SELECT_LSB, (bank + 1) & 0xff);
    m_controlChangeCallback(channel + 1, IMidiInterface::BANK_SELECT_MSB, (bank + 1) >> 8);

    m_pConcert->execute();

    // Check stored bank
    ASSERT_EQ(bank, m_pConcert->getCurrentBank());
}

TEST_F(ConcertTest, convertToJson)
{
    // Set some non-default values
    m_pConcert->setListeningToProgramChange(true);
    m_pConcert->setCurrentBank(2);
    m_pConcert->setProgramChangeChannel(3);

    Processing::TNoteToLightMap map({{1, 10}, {2, 20}});
    m_pConcert->setNoteToLightMap(map);
    
    json mockPatchJson, mockPatch2Json;
    mockPatchJson["objectType"] = "MockPatch";
    mockPatchJson["someParameter"] = 42;
    mockPatch2Json["objectType"] = "MockPatch";
    mockPatch2Json["someParameter"] = 43;

    MockPatch* pMockPatch = new MockPatch();
    ASSERT_NE(nullptr, pMockPatch);
    MockPatch* pMockPatch2 = new MockPatch();
    ASSERT_NE(nullptr, pMockPatch2);

    EXPECT_CALL(m_mockProcessingBlockFactory, createPatch())
        .WillOnce(Return(pMockPatch))
        .WillOnce(Return(pMockPatch2));

    EXPECT_CALL(*pMockPatch, convertToJson())
        .WillOnce(Return(mockPatchJson));
    EXPECT_CALL(*pMockPatch2, convertToJson())
        .WillOnce(Return(mockPatch2Json));

    m_pConcert->addPatch();
    m_pConcert->addPatch();

    json converted = m_pConcert->convertToJson();
    EXPECT_EQ(true, converted.at("isListeningToProgramChange").get<bool>());
    EXPECT_EQ(2, converted.at("currentBank").get<uint8_t>());
    EXPECT_EQ(3, converted.at("programChangeChannel").get<uint8_t>());
    EXPECT_EQ(Processing::convert(map), converted.at("noteToLightMap").get<Processing::TStringNoteToLightMap>());
}
