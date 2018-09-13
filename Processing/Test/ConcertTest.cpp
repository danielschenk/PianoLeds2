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
#include "Drivers/Test/MidiInputObserverTest.h"

#include "../Concert.h"
#include "../Mock/MockProcessingBlockFactory.h"
#include "../Mock/MockPatch.h"

using testing::_;
using testing::SaveArg;
using testing::Return;
using testing::Expectation;
using testing::NiceMock;
using testing::SetArgReferee;

class MockObserver
    : public Concert::IObserver
{
public:
    MOCK_METHOD1(onStripUpdate, void(const Processing::TRgbStrip& strip));
};

class ConcertTest
    : public MidiInputObserverTest
{
public:
    ConcertTest()
        : MidiInputObserverTest()
        , m_mockProcessingBlockFactory()
    {
        m_concert = new Concert(m_mockMidiInput, m_mockProcessingBlockFactory);
    }

    virtual ~ConcertTest()
    {
        delete m_concert;
    }

    // Should not be default and go beyond the byte range, to test LSB and MSB
    static const uint16_t c_testBankNumber;

    // Required mocks
    NiceMock<MockProcessingBlockFactory> m_mockProcessingBlockFactory;

    // Object under test
    Concert*  m_concert;
};

const uint16_t ConcertTest::c_testBankNumber = 300;

TEST_F(ConcertTest, bankSelect)
{
    const uint8_t channel(0);
    
    m_concert->setListeningToProgramChange(true);
    m_concert->setProgramChangeChannel(channel);

    // Simulate a bank select sequence
    m_observer->onControlChange(channel, IMidiInterface::BANK_SELECT_LSB, c_testBankNumber & 0xff);
    m_observer->onControlChange(channel, IMidiInterface::BANK_SELECT_MSB, c_testBankNumber >> 8);

    m_concert->execute();

    // Check stored bank
    ASSERT_EQ(c_testBankNumber, m_concert->getCurrentBank());
}

TEST_F(ConcertTest, bankSelectFromOtherChannelIgnored)
{
    const uint8_t channel(0);
    const uint16_t bank(m_concert->getCurrentBank());
    
    m_concert->setListeningToProgramChange(true);
    m_concert->setProgramChangeChannel(channel);

    // Simulate a bank select sequence
    m_observer->onControlChange(channel + 1, IMidiInterface::BANK_SELECT_LSB, (bank + 1) & 0xff);
    m_observer->onControlChange(channel + 1, IMidiInterface::BANK_SELECT_MSB, (bank + 1) >> 8);

    m_concert->execute();

    // Check stored bank
    ASSERT_EQ(bank, m_concert->getCurrentBank());
}

TEST_F(ConcertTest, execute)
{
    Processing::TNoteToLightMap map;
    map[42] = 42;
    m_concert->setNoteToLightMap(map);

    MockPatch* mockPatch(new NiceMock<MockPatch>);
    m_concert->addPatch(mockPatch);

    MockObserver observer;
    m_concert->subscribe(observer);

    Processing::TRgbStrip newStripValues({{42, 43, 44}});

    // The mock patch should be executed, and given the configured note to light map.
    // Let the mock patch set some values on the strip during its execute
    EXPECT_CALL(*mockPatch, execute(_, map))
        .WillOnce(SetArgReferee<0>(newStripValues));

    // The new strip values should be notified
    EXPECT_CALL(observer, onStripUpdate(newStripValues));

    m_concert->execute();
}

TEST_F(ConcertTest, executeEmpty)
{
    // Should not crash
    m_concert->execute();
}

TEST_F(ConcertTest, activateFirstPatch)
{
    MockPatch* mockPatch(new NiceMock<MockPatch>);
    EXPECT_CALL(*mockPatch, activate());
    m_concert->addPatch(mockPatch);
}

TEST_F(ConcertTest, updateStripSize)
{
    Processing::TNoteToLightMap map;
    map[0] = 42;
    map[1] = 6;
    map[2] = 7;
    m_concert->setNoteToLightMap(map);

    EXPECT_EQ(43, m_concert->getStripSize());
}

TEST_F(ConcertTest, convertToJson)
{
    // Set some non-default values
    m_concert->setListeningToProgramChange(true);
    m_concert->setCurrentBank(2);
    m_concert->setProgramChangeChannel(3);

    Processing::TNoteToLightMap map({{1, 10}, {2, 20}});
    m_concert->setNoteToLightMap(map);
    
    Json::object mockPatchJson, mockPatch2Json;
    mockPatchJson["objectType"] = "MockPatch";
    mockPatchJson["someParameter"] = 42;
    mockPatch2Json["objectType"] = "MockPatch";
    mockPatch2Json["someParameter"] = 43;

    MockPatch* mockPatch = new NiceMock<MockPatch>();
    ASSERT_NE(nullptr, mockPatch);
    MockPatch* mockPatch2 = new NiceMock<MockPatch>();
    ASSERT_NE(nullptr, mockPatch2);

    EXPECT_CALL(m_mockProcessingBlockFactory, createPatch())
        .WillOnce(Return(mockPatch))
        .WillOnce(Return(mockPatch2));

    EXPECT_CALL(*mockPatch, convertToJson())
        .WillOnce(Return(mockPatchJson));
    EXPECT_CALL(*mockPatch2, convertToJson())
        .WillOnce(Return(mockPatch2Json));

    // This will use the mock factory to create the patch instances.
    m_concert->addPatch();
    m_concert->addPatch();

    Json::object converted = m_concert->convertToJson().object_items();
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

    NiceMock<MockPatch>* convertedPatch1 = new NiceMock<MockPatch>();
    NiceMock<MockPatch>* convertedPatch2 = new NiceMock<MockPatch>();

    std::string name1("Purple Rain");
    std::string name2("Simply Red");
    ON_CALL(*convertedPatch1, getName())
        .WillByDefault(Return(name1));
    ON_CALL(*convertedPatch2, getName())
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
        .WillOnce(Return(convertedPatch1));
    EXPECT_CALL(m_mockProcessingBlockFactory, createPatch(Json(mockPatch2Json)))
        .After(first)
        .WillOnce(Return(convertedPatch2));

    m_concert->convertFromJson(j);
    EXPECT_EQ(true, m_concert->isListeningToProgramChange());
    EXPECT_EQ(2, m_concert->getCurrentBank());
    EXPECT_EQ(3, m_concert->getProgramChangeChannel());

    ASSERT_EQ(2, m_concert->size());
    EXPECT_EQ(name1, m_concert->getPatch(0)->getName());
    EXPECT_EQ(name2, m_concert->getPatch(1)->getName());

    Processing::TNoteToLightMap expectedMap;
    expectedMap[1] = 10;
    expectedMap[2] = 20;
    EXPECT_EQ(expectedMap, m_concert->getNoteToLightMap());
    EXPECT_EQ(21, m_concert->getStripSize());
}
