/**
 * @file
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 *
 * @brief Unit test for NoteRgbSource.
 */

#include <gtest/gtest.h>
#include <json.hpp>
#include <Drivers/Mock/MockMidiInput.h>
#include <Common/Mock/LoggingTest.h>

#include "../NoteRgbSource.h"
#include "../Mock/MockRgbFunction.h"
#include "../Mock/MockRgbFunctionFactory.h"

using ::testing::_;
using ::testing::SaveArg;
using ::testing::Return;
using ::testing::HasSubstr;

#define LOGGING_COMPONENT "NoteRgbSource"

#include <cstdint>

class NoteRgbSourceTest
    : public LoggingTest
{
public:
    static constexpr unsigned int c_StripSize = 10;

    NoteRgbSourceTest()
        : LoggingTest()
        , m_mockMidiInput()
        , m_mockRgbFunctionFactory()
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
             })"_json)
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
        m_pNoteRgbSource = new NoteRgbSource(m_mockMidiInput, m_noteToLightMap, m_mockRgbFunctionFactory);
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
    MockRgbFunctionFactory m_mockRgbFunctionFactory;
    NoteRgbSource* m_pNoteRgbSource;
    Processing::TRgbStrip m_strip;

    IMidiInput::TNoteOnOffFunction m_noteOnOffCallback;
    IMidiInput::TControlChangeFunction m_controlChangeCallback;

    Processing::TNoteToLightMap m_noteToLightMap;

    json m_exampleJson;
};

class JsonPropertyNoteRgbSourceTest
    : public NoteRgbSourceTest
    , public ::testing::WithParamInterface<std::string>
{
};

INSTANTIATE_TEST_CASE_P(KnownJsonProperties,
                        JsonPropertyNoteRgbSourceTest,
                        ::testing::Values("usingPedal", "channel", "rgbFunction"));

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
    m_pNoteRgbSource->setUsingPedal(false);

    auto darkStrip = m_strip;

    m_noteOnOffCallback(0, 0, 1, true);
    m_controlChangeCallback(0, IMidiInterface::DAMPER_PEDAL, 0xff);
    m_noteOnOffCallback(0, 0, 1, false);

    m_pNoteRgbSource->execute(m_strip);
    EXPECT_EQ(darkStrip, m_strip);
}

TEST_F(NoteRgbSourceTest, usePedal)
{
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
    MockRgbFunction* pMockRgbFunction = new MockRgbFunction();
    EXPECT_CALL(*pMockRgbFunction, calculate(_, _))
        .WillRepeatedly(ReturnBlueWhenNoteSoundingOtherwiseRed());
    m_pNoteRgbSource->setRgbFunction(pMockRgbFunction);

    // (channel, number, velocity, on/off)
    m_noteOnOffCallback(0, 0, 1, true);
    m_noteOnOffCallback(0, 5, 6, true);

    m_pNoteRgbSource->execute(m_strip);

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

TEST_F(NoteRgbSourceTest, deleteRgbFunction)
{
    MockRgbFunction* pMock1 = new MockRgbFunction();
    MockRgbFunction* pMock2 = new MockRgbFunction();

    // Need to set an action, to make Google Test throw an error in case of a leaked mock.
    ON_CALL(*pMock1, calculate(_, _))
        .WillByDefault(Return(Processing::TRgb()));
    ON_CALL(*pMock2, calculate(_, _))
        .WillByDefault(Return(Processing::TRgb()));

    m_pNoteRgbSource->setRgbFunction(pMock1);
    m_pNoteRgbSource->setRgbFunction(pMock2);
}

TEST_F(NoteRgbSourceTest, convertToJson)
{
    MockRgbFunction* pMockRgbFunction = new MockRgbFunction();
    ASSERT_NE(nullptr, pMockRgbFunction);

    json mockRgbFunctionJson;
    mockRgbFunctionJson["objectType"] = "MockRgbFunction";
    mockRgbFunctionJson["someParameter"] = 42;
    EXPECT_CALL(*pMockRgbFunction, convertToJson())
        .WillOnce(Return(mockRgbFunctionJson));

    // Set some non-default values
    m_pNoteRgbSource->setRgbFunction(pMockRgbFunction);
    m_pNoteRgbSource->setChannel(6);
    m_pNoteRgbSource->setUsingPedal(false);

    json j = m_pNoteRgbSource->convertToJson();
    EXPECT_EQ("NoteRgbSource", j.at("objectType").get<std::string>());
    EXPECT_EQ(6, j.at("channel").get<int>());
    EXPECT_EQ(false, j.at("usingPedal").get<bool>());
    EXPECT_EQ("MockRgbFunction", j.at("rgbFunction").at("objectType").get<std::string>());
    EXPECT_EQ(42, j.at("rgbFunction").at("someParameter").get<int>());
}

TEST_F(NoteRgbSourceTest, convertFromJson)
{
    json j(m_exampleJson);
    json mockRgbFunctionJson;
    mockRgbFunctionJson["objectType"] = "MockRgbFunction";
    mockRgbFunctionJson["someParameter"] = 42;

    MockRgbFunction* pMockRgbFunction = new MockRgbFunction();
    ASSERT_NE(nullptr, pMockRgbFunction);
    EXPECT_CALL(*pMockRgbFunction, calculate(_, _))
        .WillRepeatedly(Return(Processing::TRgb(1, 2, 3)));

    EXPECT_CALL(m_mockRgbFunctionFactory, createRgbFunction(mockRgbFunctionJson))
        .WillOnce(Return(pMockRgbFunction));

    m_pNoteRgbSource->convertFromJson(j);
    EXPECT_EQ(6, m_pNoteRgbSource->getChannel());
    EXPECT_EQ(false, m_pNoteRgbSource->isUsingPedal());

    Processing::TRgbStrip reference(3);
    reference[0] = {1, 2, 3};
    reference[1] = {1, 2, 3};
    reference[2] = {1, 2, 3};
    Processing::TRgbStrip testStrip(3);
    m_pNoteRgbSource->execute(testStrip);
    EXPECT_EQ(reference, testStrip);
}

TEST_P(JsonPropertyNoteRgbSourceTest, convertFromJsonMissingProperty)
{
    json j(m_exampleJson);

    ASSERT_EQ(1, j.erase(GetParam()));

    if(GetParam() != "rgbFunction")
    {
        // RGB function is not deleted from JSON, so expect call to factory
        MockRgbFunction* pMockRgbFunction = new MockRgbFunction();
        ASSERT_NE(nullptr, pMockRgbFunction);

        EXPECT_CALL(m_mockRgbFunctionFactory, createRgbFunction(_))
            .WillOnce(Return(pMockRgbFunction));
    }

    EXPECT_CALL(m_mockLoggingTarget, logMessage(_, Logging::LogLevel_Error, LOGGING_COMPONENT, HasSubstr(GetParam())));
    m_pNoteRgbSource->convertFromJson(j);
}
