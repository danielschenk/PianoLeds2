/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
 * 
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
 *
 * @brief Unit test for NoteRgbSource.
 */

#include "../NoteRgbSource.h"
#include "../Mock/MockRgbFunction.h"
#include "../Mock/MockRgbFunctionFactory.h"

#include "gtest/gtest.h"
#include "Test/MidiInputObserverTest.h"
#include "Mock/LoggingTest.h"
#include "Mock/MockTime.h"


using ::testing::_;
using ::testing::SaveArg;
using ::testing::Return;
using ::testing::HasSubstr;
using ::testing::Each;
using ::testing::InSequence;
using ::testing::AnyNumber;

#define LOGGING_COMPONENT "NoteRgbSource"

class NoteRgbSourceTest
    : public LoggingTest
    , public MidiInputObserverTest
{
public:
    static constexpr unsigned int c_StripSize = 10;

    NoteRgbSourceTest()
        : LoggingTest()
        , MidiInputObserverTest()
        , m_mockRgbFunctionFactory()
        , m_mockTime()
        , m_strip(c_StripSize)
        , m_exampleJson(R"(
             {
                 "objectType": "NoteRgbSource",
                 "channel": 6,
                 "usingPedal": false,
                 "rgbFunction": {
                     "objectType": "MockRgbFunction",
                     "someParameter": 42
                 }
             })")
    {
        for(int i = 0; i < c_StripSize; ++i)
        {
            // Default: simple 1-to-1 mapping
            m_noteToLightMap[i] = i;
        }
        m_noteRgbSource = new NoteRgbSource(m_mockMidiInput, m_mockRgbFunctionFactory,
            m_mockTime);
        m_noteRgbSource->activate();
   }

    virtual ~NoteRgbSourceTest()
    {
        delete m_noteRgbSource;
    }

    void resetStrip()
    {
        for(auto& color : m_strip)
        {
            color.r = 0;
            color.g = 0;
            color.b = 0;
        }
    }

    MockRgbFunctionFactory m_mockRgbFunctionFactory;
    NiceMock<MockTime> m_mockTime;
    NoteRgbSource* m_noteRgbSource;
    Processing::TRgbStrip m_strip;

    Processing::TNoteToLightMap m_noteToLightMap;

    std::string m_exampleJson;
};

TEST_F(NoteRgbSourceTest, noNotesSounding)
{
    m_strip[0] = {4, 5, 6};
    m_strip[6] = {7, 8, 9};
    m_strip[c_StripSize-1] = {11, 12, 13};

    auto expectedStrip(m_strip);

    m_noteRgbSource->execute(m_strip, m_noteToLightMap);

    // No notes sounding should leave strip untouched
    ASSERT_EQ(expectedStrip, m_strip);
}

TEST_F(NoteRgbSourceTest, noteOn)
{
    // (channel, number, velocity, on/off)
    m_observer->onNoteChange(0, 0, 1, true);
    m_observer->onNoteChange(0, 5, 6, true);

    m_noteRgbSource->execute(m_strip, m_noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto reference = Processing::TRgbStrip(c_StripSize);
    reference[0] = {0xff, 0xff, 0xff};
    reference[5] = {0xff, 0xff, 0xff};

    EXPECT_EQ(reference, m_strip);
}

TEST_F(NoteRgbSourceTest, deactivateDisablesAllNotes)
{
    // (channel, number, velocity, on/off)
    m_noteRgbSource->deactivate();
    m_observer->onNoteChange(0, 0, 1, true);
    m_observer->onNoteChange(0, 5, 6, true);

    m_noteRgbSource->execute(m_strip, m_noteToLightMap);

    EXPECT_THAT(m_strip, Each(Processing::TRgb({0, 0, 0})));
}

TEST_F(NoteRgbSourceTest, noteOff)
{
    // (channel, number, velocity, on/off)
    m_observer->onNoteChange(0, 0, 1, true);
    m_observer->onNoteChange(0, 5, 6, true);

    m_observer->onNoteChange(0, 0, 8, false);

    m_noteRgbSource->execute(m_strip, m_noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto reference = Processing::TRgbStrip(c_StripSize);
    reference[5] = {0xff, 0xff, 0xff};

    EXPECT_EQ(reference, m_strip);
}

TEST_F(NoteRgbSourceTest, ignoreOtherChannel)
{
    m_observer->onNoteChange(1, 0, 1, true);

    m_noteRgbSource->execute(m_strip, m_noteToLightMap);
    EXPECT_THAT(m_strip, Each(Processing::TRgb({0, 0, 0})));
}

TEST_F(NoteRgbSourceTest, ignorePedal)
{
    m_noteRgbSource->setUsingPedal(false);

    m_observer->onNoteChange(0, 0, 1, true);
    m_observer->onControlChange(0, IMidiInterface::DAMPER_PEDAL, 0xff);
    m_observer->onNoteChange(0, 0, 1, false);

    m_noteRgbSource->execute(m_strip, m_noteToLightMap);
    EXPECT_THAT(m_strip, Each(Processing::TRgb({0, 0, 0})));
}

TEST_F(NoteRgbSourceTest, usePedal)
{
    // Press a key
    // (channel, number, velocity, on/off)
    m_observer->onNoteChange(0, 0, 1, true);
    // Press pedal
    // (channel, number, value)
    m_observer->onControlChange(0, IMidiInterface::DAMPER_PEDAL, 0xff);
    // Press another key
    m_observer->onNoteChange(0, 2, 1, true);

    auto reference = Processing::TRgbStrip(c_StripSize);
    // Both notes are still sounding
    reference[0] = {0xff, 0xff, 0xff};
    reference[2] = {0xff, 0xff, 0xff};
    m_noteRgbSource->execute(m_strip, m_noteToLightMap);
    EXPECT_EQ(reference, m_strip);

    // Release keys
    m_observer->onNoteChange(0, 0, 1, false);
    m_observer->onNoteChange(0, 2, 1, false);

    // Both notes are still sounding
    resetStrip();
    m_noteRgbSource->execute(m_strip, m_noteToLightMap);
    EXPECT_EQ(reference, m_strip);

    // Release pedal
    m_observer->onControlChange(0, IMidiInterface::DAMPER_PEDAL, 0);

    // Notes are not sounding anymore
    reference[0] = {0, 0, 0};
    reference[2] = {0, 0, 0};
    resetStrip();
    m_noteRgbSource->execute(m_strip, m_noteToLightMap);
    EXPECT_EQ(reference, m_strip);
}

/** Action definition for mock RGB function. */
ACTION(ReturnBlueWhenNoteSoundingOtherwiseRed)
{
    Processing::TRgb output;
    if(arg0.sounding)
    {
        output.b = 1;
    }
    else
    {
        output.r = 1;
    }

    return output;
}

TEST_F(NoteRgbSourceTest, otherRgbFunction)
{
    MockRgbFunction* mockRgbFunction = new MockRgbFunction();
    EXPECT_CALL(*mockRgbFunction, calculate(_, _))
        .WillRepeatedly(ReturnBlueWhenNoteSoundingOtherwiseRed());
    m_noteRgbSource->setRgbFunction(mockRgbFunction);

    // (channel, number, velocity, on/off)
    m_observer->onNoteChange(0, 0, 1, true);
    m_observer->onNoteChange(0, 5, 6, true);

    m_noteRgbSource->execute(m_strip, m_noteToLightMap);

    auto reference = Processing::TRgbStrip(c_StripSize);
    reference[0] = {0, 0, 1};
    reference[1] = {1, 0, 0};
    reference[2] = {1, 0, 0};
    reference[3] = {1, 0, 0};
    reference[4] = {1, 0, 0};
    reference[5] = {0, 0, 1};
    reference[6] = {1, 0, 0};
    reference[7] = {1, 0, 0};
    reference[8] = {1, 0, 0};
    reference[9] = {1, 0, 0};

    EXPECT_EQ(reference, m_strip);
}

TEST_F(NoteRgbSourceTest, timePassedToRgbFunction)
{
    EXPECT_CALL(m_mockTime, getMilliseconds())
        .WillOnce(Return(42))
        .WillOnce(Return(43))
        .WillRepeatedly(Return(44));

    MockRgbFunction* mockRgbFunction = new MockRgbFunction();
    {
        InSequence dummy;
        EXPECT_CALL(*mockRgbFunction, calculate(_, 42));
        EXPECT_CALL(*mockRgbFunction, calculate(_, 43));
        EXPECT_CALL(*mockRgbFunction, calculate(_, 44))
            .Times(AnyNumber());
        m_noteRgbSource->setRgbFunction(mockRgbFunction);

        m_noteRgbSource->execute(m_strip, m_noteToLightMap);
    }
}

TEST_F(NoteRgbSourceTest, otherNoteToLightMap)
{
    m_noteToLightMap.clear();
    m_noteToLightMap[0] = 9;
    m_noteToLightMap[5] = 8;

    // (channel, number, velocity, on/off)
    m_observer->onNoteChange(0, 0, 1, true);
    m_observer->onNoteChange(0, 5, 6, true);

    m_noteRgbSource->execute(m_strip, m_noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto reference = Processing::TRgbStrip(c_StripSize);
    reference[9] = {0xff, 0xff, 0xff};
    reference[8] = {0xff, 0xff, 0xff};

    EXPECT_EQ(reference, m_strip);
}

TEST_F(NoteRgbSourceTest, doNotWriteOutsideStrip)
{
    // (channel, number, velocity, on/off)
    m_observer->onNoteChange(0, 0, 1, true);
    m_observer->onNoteChange(0, 9, 6, true);

    auto shorterStrip = Processing::TRgbStrip(5);
    m_noteRgbSource->execute(shorterStrip, m_noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto reference = Processing::TRgbStrip(5);
    reference[0] = {0xff, 0xff, 0xff};

    EXPECT_EQ(reference, shorterStrip);
}

TEST_F(NoteRgbSourceTest, deleteRgbFunction)
{
    MockRgbFunction* mock1 = new MockRgbFunction();
    MockRgbFunction* mock2 = new MockRgbFunction();

    // Need to set an action, to make Google Test throw an error in case of a leaked mock.
    ON_CALL(*mock1, calculate(_, _))
        .WillByDefault(Return(Processing::TRgb()));
    ON_CALL(*mock2, calculate(_, _))
        .WillByDefault(Return(Processing::TRgb()));

    m_noteRgbSource->setRgbFunction(mock1);
    m_noteRgbSource->setRgbFunction(mock2);
}

TEST_F(NoteRgbSourceTest, convertToJson)
{
    MockRgbFunction* mockRgbFunction = new MockRgbFunction();
    ASSERT_NE(nullptr, mockRgbFunction);

    Json::object mockRgbFunctionJson;
    mockRgbFunctionJson["objectType"] = "MockRgbFunction";
    mockRgbFunctionJson["someParameter"] = 42;
    EXPECT_CALL(*mockRgbFunction, convertToJson())
        .WillOnce(Return(mockRgbFunctionJson));

    // Set some non-default values
    m_noteRgbSource->setRgbFunction(mockRgbFunction);
    m_noteRgbSource->setChannel(6);
    m_noteRgbSource->setUsingPedal(false);

    Json::object j = m_noteRgbSource->convertToJson().object_items();
    EXPECT_EQ("NoteRgbSource", j.at("objectType").string_value());
    EXPECT_EQ(6, j.at("channel").int_value());
    EXPECT_EQ(false, j.at("usingPedal").bool_value());
    EXPECT_EQ("MockRgbFunction", j.at("rgbFunction").object_items().at("objectType").string_value());
    EXPECT_EQ(42, j.at("rgbFunction").object_items().at("someParameter").int_value());
}

TEST_F(NoteRgbSourceTest, convertFromJson)
{
    std::string err;
    Json j(Json::parse(m_exampleJson, err, json11::STANDARD));
    Json::object mockRgbFunctionJson;
    mockRgbFunctionJson["objectType"] = "MockRgbFunction";
    mockRgbFunctionJson["someParameter"] = 42;

    MockRgbFunction* mockRgbFunction = new MockRgbFunction();
    ASSERT_NE(nullptr, mockRgbFunction);
    EXPECT_CALL(*mockRgbFunction, calculate(_, _))
        .WillRepeatedly(Return(Processing::TRgb(1, 2, 3)));

    EXPECT_CALL(m_mockRgbFunctionFactory, createRgbFunction(Json(mockRgbFunctionJson)))
        .WillOnce(Return(mockRgbFunction));

    m_noteRgbSource->convertFromJson(j);
    EXPECT_EQ(6, m_noteRgbSource->getChannel());
    EXPECT_EQ(false, m_noteRgbSource->isUsingPedal());

    Processing::TRgbStrip reference(3);
    reference[0] = {1, 2, 3};
    reference[1] = {1, 2, 3};
    reference[2] = {1, 2, 3};
    Processing::TRgbStrip testStrip(3);
    m_noteRgbSource->execute(testStrip, m_noteToLightMap);
    EXPECT_EQ(reference, testStrip);
}
