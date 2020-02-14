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

#include <gtest/gtest.h>

#include "../Patch.h"
#include "../Mock/MockProcessingChain.h"
#include "../Mock/MockProcessingBlockFactory.h"

using ::testing::Return;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::Invoke;

class PatchTest
    : public ::testing::Test
{
public:
    PatchTest()
        : processingBlockFactory()
        , processingChain(new NiceMock<MockProcessingChain>)
    {
        ON_CALL(processingBlockFactory, createProcessingChain())
            .WillByDefault(Return(processingChain));

        patch = new Patch(processingBlockFactory);
    }

    virtual ~PatchTest()
    {
        delete patch;

        // deleted by Patch
        processingChain = nullptr;
    }

    NiceMock<MockProcessingBlockFactory> processingBlockFactory;
    NiceMock<MockProcessingChain>* processingChain;
    Patch* patch;
};

TEST_F(PatchTest, defaults)
{
    EXPECT_EQ(0, patch->getBank());
    EXPECT_EQ(0, patch->getProgram());
    EXPECT_EQ(false, patch->hasBankAndProgram());
    EXPECT_EQ("Untitled Patch", patch->getName());
}

TEST_F(PatchTest, convertToJson)
{
    // Set some non-defaults
    patch->setBank(42);
    patch->setProgram(43);
    patch->setName("Awesome patch");

    Json::object mockChainJson;
    mockChainJson["objectType"] = "mockChain";
    mockChainJson["someProperty"] = 42;
    EXPECT_CALL(*processingChain, convertToJson())
        .WillOnce(Return(mockChainJson));

    Json::object converted = patch->convertToJson().object_items();
    EXPECT_EQ(42, converted.at("bank").number_value());
    EXPECT_EQ(43, converted.at("program").number_value());
    EXPECT_EQ(true, converted.at("hasBankAndProgram").bool_value());
    EXPECT_EQ("Awesome patch", converted.at("name").string_value());

    EXPECT_EQ(mockChainJson, converted["processingChain"].object_items());
    EXPECT_EQ("Patch", converted.at("objectType").string_value());
}

TEST_F(PatchTest, convertFromJson)
{
    Json::object mockChainJson;
    mockChainJson["objectType"] = "mockChain";
    mockChainJson["someProperty"] = 42;

    Json::object j;
    j["processingChain"] = mockChainJson;
    j["bank"] = 42;
    j["program"] = 43;
    j["hasBankAndProgram"] = true;
    j["name"] = std::string("Awesome patch");

    EXPECT_CALL(*processingChain, convertFromJson(Json(mockChainJson)));
    patch->convertFromJson(Json(j));

    EXPECT_EQ(42, patch->getBank());
    EXPECT_EQ(43, patch->getProgram());
    EXPECT_EQ(true, patch->hasBankAndProgram());
    EXPECT_EQ("Awesome patch", patch->getName());
}

TEST_F(PatchTest, activate)
{
    EXPECT_CALL(*processingChain, activate());
    patch->activate();
}

TEST_F(PatchTest, deactivate)
{
    EXPECT_CALL(*processingChain, deactivate());
    patch->deactivate();
}

TEST_F(PatchTest, execute)
{
    Processing::TRgbStrip strip;
    strip.push_back(Processing::TRgb({0, 0, 0}));

    // Pass a map with something we can verify
    Processing::TNoteToLightMap map;
    map[42] = 42;

    // Let the mock processing chain do something with the strip which we can verify
    Processing::TRgb valueAfterProcessing({1, 2, 3});
    ASSERT_NE(valueAfterProcessing, strip[0]);

    EXPECT_CALL(*processingChain, execute(_, map))
        .WillOnce(Invoke([valueAfterProcessing](Processing::TRgbStrip& strip, const Processing::TNoteToLightMap&){
            strip[0] = valueAfterProcessing;
    }));

    patch->execute(strip, map);

    EXPECT_EQ(valueAfterProcessing, strip[0]);
}
