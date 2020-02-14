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
#include "Test/MidiInputObserverTest.h"

#include "../Concert.h"
#include "../Mock/MockProcessingBlockFactory.h"
#include "../Mock/MockPatch.h"
#include "Mock/MockTime.h"
#include "LoggingEntryPoint.h"

using testing::_;
using testing::SaveArg;
using testing::Return;
using testing::ReturnNew;
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
        , mockProcessingBlockFactory()
        , mockTime()
    {
        LoggingEntryPoint::setTime(&mockTime);
        concert = new Concert(mockMidiInput, mockProcessingBlockFactory);

        ON_CALL(mockProcessingBlockFactory, createPatch())
            .WillByDefault(ReturnNew<NiceMock<MockPatch>>());
    }

    virtual ~ConcertTest()
    {
        delete concert;
    }

    void sendBankSelectSequence(uint8_t channel, uint16_t bank)
    {
        concert->onControlChange(channel, IMidiInterface::BANK_SELECT_LSB, bank & 0x7f);
        concert->onControlChange(channel, IMidiInterface::BANK_SELECT_MSB, bank >> 7);
    }

    // Should not be default and go beyond the byte range, to test LSB and MSB
    static const uint16_t c_testBankNumber;

    // Required mocks
    NiceMock<MockProcessingBlockFactory> mockProcessingBlockFactory;
    NiceMock<MockTime> mockTime;

    // Object under test
    Concert*  concert;
};

const uint16_t ConcertTest::c_testBankNumber = 129;

TEST_F(ConcertTest, bankSelect)
{
    const uint8_t channel(0);
    
    concert->setListeningToProgramChange(true);
    concert->setProgramChangeChannel(channel);

    // Simulate a bank select sequence
    sendBankSelectSequence(channel, c_testBankNumber);
    concert->execute();

    // Check stored bank
    ASSERT_EQ(c_testBankNumber, concert->getCurrentBank());
}

TEST_F(ConcertTest, bankSelectFromOtherChannelIgnored)
{
    const uint8_t channel(0);
    const uint16_t bank(concert->getCurrentBank());
    
    concert->setListeningToProgramChange(true);
    concert->setProgramChangeChannel(channel);

    // Simulate a bank select sequence
    sendBankSelectSequence(channel + 1, bank + 1);

    concert->execute();

    // Check stored bank
    ASSERT_EQ(bank, concert->getCurrentBank());
}

TEST_F(ConcertTest, execute)
{
    Processing::TNoteToLightMap map;
    map[42] = 42;
    concert->setNoteToLightMap(map);

    MockPatch* mockPatch(new NiceMock<MockPatch>);
    concert->addPatch(mockPatch);

    MockObserver observer;
    concert->subscribe(observer);

    Processing::TRgbStrip newStripValues({{42, 43, 44}});

    // The mock patch should be executed, and given the configured note to light map.
    // Let the mock patch set some values on the strip during its execute
    EXPECT_CALL(*mockPatch, execute(_, map))
        .WillOnce(SetArgReferee<0>(newStripValues));

    // The new strip values should be notified
    EXPECT_CALL(observer, onStripUpdate(newStripValues));

    concert->execute();
}

TEST_F(ConcertTest, executeWithMultiplePatches)
{
    auto mockPatch(new NiceMock<MockPatch>);
    concert->addPatch(mockPatch);

    auto mockPatch2(new NiceMock<MockPatch>);
    concert->addPatch(mockPatch2);

    EXPECT_CALL(*mockPatch, execute(_, _));
    EXPECT_CALL(*mockPatch2, execute(_, _))
        .Times(0);

    concert->execute();
}

TEST_F(ConcertTest, executeEmpty)
{
    // Should not crash
    concert->execute();
}

TEST_F(ConcertTest, activateFirstPatch)
{
    auto mockPatch(new NiceMock<MockPatch>);
    EXPECT_CALL(*mockPatch, activate());
    concert->addPatch(mockPatch);
}

TEST_F(ConcertTest, patchChangeOnProgramChange)
{
    uint8_t program(42);

    auto mockPatch(new NiceMock<MockPatch>);
    auto mockPatch2(new NiceMock<MockPatch>);
    ON_CALL(*mockPatch2, getBank())
        .WillByDefault(Return(c_testBankNumber));
    ON_CALL(*mockPatch2, getProgram())
        .WillByDefault(Return(program));
    ON_CALL(*mockPatch2, hasBankAndProgram())
        .WillByDefault(Return(true));

    concert->addPatch(mockPatch);
    concert->addPatch(mockPatch2);

    EXPECT_CALL(*mockPatch, deactivate());
    EXPECT_CALL(*mockPatch2, activate());
    EXPECT_CALL(*mockPatch2, execute(_, _));

    uint8_t channel(2);
    concert->setListeningToProgramChange(true);
    concert->setProgramChangeChannel(channel);
    sendBankSelectSequence(channel, c_testBankNumber);
    concert->onProgramChange(channel, program);
    concert->execute();
}

TEST_F(ConcertTest, addPatch)
{
    EXPECT_EQ(0, concert->addPatch());
    EXPECT_EQ(1, concert->addPatch());
    EXPECT_EQ(2, concert->addPatch(new NiceMock<MockPatch>));
}

TEST_F(ConcertTest, getPatch)
{
    auto mockPatch(new NiceMock<MockPatch>);
    ON_CALL(*mockPatch, getName())
        .WillByDefault(Return("first"));

    auto mockPatch2(new NiceMock<MockPatch>);
    ON_CALL(*mockPatch2, getName())
        .WillByDefault(Return("second"));

    concert->addPatch(mockPatch);
    concert->addPatch(mockPatch2);

    EXPECT_EQ("first", concert->getPatch(0)->getName());
    EXPECT_EQ("second", concert->getPatch(1)->getName());
}

TEST_F(ConcertTest, updateStripSize)
{
    Processing::TNoteToLightMap map;
    map[0] = 42;
    map[1] = 6;
    map[2] = 7;
    concert->setNoteToLightMap(map);

    EXPECT_EQ(43, concert->getStripSize());
}

TEST_F(ConcertTest, convertToJson)
{
    // Set some non-default values
    concert->setListeningToProgramChange(true);
    concert->setCurrentBank(2);
    concert->setProgramChangeChannel(3);

    Processing::TNoteToLightMap map({{1, 10}, {2, 20}});
    concert->setNoteToLightMap(map);
    
    Json::object mockPatchJson, mockPatch2Json;
    mockPatchJson["objectType"] = "MockPatch";
    mockPatchJson["someParameter"] = 42;
    mockPatch2Json["objectType"] = "MockPatch";
    mockPatch2Json["someParameter"] = 43;

    MockPatch* mockPatch = new NiceMock<MockPatch>();
    ASSERT_NE(nullptr, mockPatch);
    MockPatch* mockPatch2 = new NiceMock<MockPatch>();
    ASSERT_NE(nullptr, mockPatch2);

    EXPECT_CALL(mockProcessingBlockFactory, createPatch())
        .WillOnce(Return(mockPatch))
        .WillOnce(Return(mockPatch2));

    EXPECT_CALL(*mockPatch, convertToJson())
        .WillOnce(Return(mockPatchJson));
    EXPECT_CALL(*mockPatch2, convertToJson())
        .WillOnce(Return(mockPatch2Json));

    // This will use the mock factory to create the patch instances.
    concert->addPatch();
    concert->addPatch();

    Json::object converted = concert->convertToJson().object_items();
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

    Expectation first = EXPECT_CALL(mockProcessingBlockFactory, createPatch(Json(mockPatch1Json)))
        .WillOnce(Return(convertedPatch1));
    EXPECT_CALL(mockProcessingBlockFactory, createPatch(Json(mockPatch2Json)))
        .After(first)
        .WillOnce(Return(convertedPatch2));

    concert->convertFromJson(j);
    EXPECT_EQ(true, concert->isListeningToProgramChange());
    EXPECT_EQ(2, concert->getCurrentBank());
    EXPECT_EQ(3, concert->getProgramChangeChannel());

    ASSERT_EQ(2, concert->size());
    EXPECT_EQ(name1, concert->getPatch(0)->getName());
    EXPECT_EQ(name2, concert->getPatch(1)->getName());

    Processing::TNoteToLightMap expectedMap;
    expectedMap[1] = 10;
    expectedMap[2] = 20;
    EXPECT_EQ(expectedMap, concert->getNoteToLightMap());
    EXPECT_EQ(21, concert->getStripSize());
}
