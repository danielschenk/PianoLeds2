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
using testing::Expectation;
using testing::NiceMock;

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

    // This will use the mock factory to create the patch instances.
    m_pConcert->addPatch();
    m_pConcert->addPatch();

    json converted = m_pConcert->convertToJson();
    EXPECT_EQ(true, converted.at("isListeningToProgramChange").get<bool>());
    EXPECT_EQ(2, converted.at("currentBank").get<uint8_t>());
    EXPECT_EQ(3, converted.at("programChangeChannel").get<uint8_t>());
    EXPECT_EQ(Processing::convert(map), converted.at("noteToLightMap").get<Processing::TStringNoteToLightMap>());
    
    json patches = converted.at("patches");
    EXPECT_EQ(2, patches.size());
    EXPECT_EQ(42, patches.at(0).at("someParameter").get<int>());
    EXPECT_EQ(43, patches.at(1).at("someParameter").get<int>());
}

TEST_F(ConcertTest, convertFromJson)
{
    json j(R"({
                "objectType": "Concert",
                "isListeningToProgramChange": true,
                "currentBank": 2,
                "programChangeChannel": 3,
                "noteToLightMap": {
                    "1": 10,
                    "2": 20
                },
                "patches": [
                    {
                        "objectType": "MockPatch",
                        "someParameter": 42
                    },
                    {
                        "objectType": "MockPatch",
                        "someParameter": 43
                    }
                ]
            })"_json);

    NiceMock<MockPatch>* pConvertedPatch1 = new NiceMock<MockPatch>();
    NiceMock<MockPatch>* pConvertedPatch2 = new NiceMock<MockPatch>();

    std::string name1("Purple Rain");
    std::string name2("Simply Red");
    ON_CALL(*pConvertedPatch1, getName())
        .WillByDefault(Return(name1));
    ON_CALL(*pConvertedPatch2, getName())
        .WillByDefault(Return(name2));

    // Re-create the sub-objects of the above test input, 
    // so we can verify that they are passed to the factory in order.
    json mockPatch1Json;
    mockPatch1Json["objectType"] = "MockPatch";
    mockPatch1Json["someParameter"] = 42;
    json mockPatch2Json;
    mockPatch2Json["objectType"] = "MockPatch";
    mockPatch2Json["someParameter"] = 43;

    Expectation first = EXPECT_CALL(m_mockProcessingBlockFactory, createPatch(mockPatch1Json))
        .WillOnce(Return(pConvertedPatch1));
    EXPECT_CALL(m_mockProcessingBlockFactory, createPatch(mockPatch2Json))
        .After(first)
        .WillOnce(Return(pConvertedPatch2));

    m_pConcert->convertFromJson(j);
    EXPECT_EQ(true, m_pConcert->isListeningToProgramChange());
    EXPECT_EQ(2, m_pConcert->getCurrentBank());
    EXPECT_EQ(3, m_pConcert->getProgramChangeChannel());

    ASSERT_EQ(2, m_pConcert->size());
    EXPECT_EQ(name1, m_pConcert->getPatch(0)->getName());
    EXPECT_EQ(name2, m_pConcert->getPatch(1)->getName());
}
