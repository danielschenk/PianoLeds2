/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) Daniel Schenk, 2017
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
    
    Json::object mockPatchJson, mockPatch2Json;
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

    Json::object converted = m_pConcert->convertToJson().object_items();
    EXPECT_EQ(true, converted.at("isListeningToProgramChange").bool_value());
    EXPECT_EQ(2, converted.at("currentBank").number_value());
    EXPECT_EQ(3, converted.at("programChangeChannel").number_value());
    EXPECT_EQ(Processing::convert(map), converted.at("noteToLightMap").object_items());
    
    Json::array patches = converted.at("patches").array_items();
    EXPECT_EQ(2, patches.size());
    EXPECT_EQ(42, patches.at(0).object_items().at("someParameter").number_value());
    EXPECT_EQ(43, patches.at(1).object_items().at("someParameter").number_value());
}

TEST_F(ConcertTest, convertFromJson)
{
    std::string err;
    Json j(Json::parse(R"({
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
            })",
            err,
            json11::STANDARD));

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
    Json::object mockPatch1Json;
    mockPatch1Json["objectType"] = "MockPatch";
    mockPatch1Json["someParameter"] = 42;
    Json::object mockPatch2Json;
    mockPatch2Json["objectType"] = "MockPatch";
    mockPatch2Json["someParameter"] = 43;

    Expectation first = EXPECT_CALL(m_mockProcessingBlockFactory, createPatch(Json(mockPatch1Json)))
        .WillOnce(Return(pConvertedPatch1));
    EXPECT_CALL(m_mockProcessingBlockFactory, createPatch(Json(mockPatch2Json)))
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
