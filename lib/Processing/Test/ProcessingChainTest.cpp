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
 * @brief Unit tests for ProcessingBlock.
 */

#include <string>
#include <vector>
#include <gtest/gtest.h>

#include <Mock/LoggingTest.h>

#include "ProcessingBlockContainerTest.h"
#include "ProcessingChain.h"
#include "ProcessingTypes.h"

#define LOGGING_COMPONENT "ProcessingChain"

using ::testing::Return;
using ::testing::HasSubstr;

class ProcessingChainTest
    : public ProcessingBlockContainerTest
{
public:
    ProcessingChainTest()
        : processingChain(processingBlockFactory)
    {
    }

    virtual ~ProcessingChainTest()
    {
    }

    ProcessingChain processingChain;
};

TEST_F(ProcessingChainTest, empty)
{
    auto testStrip = Processing::TRgbStrip(c_stripSize);
    testStrip[0] = { 1, 0, 0 };
    testStrip[1] = { 0, 1, 0 };
    testStrip[2] = { 0, 0, 1 };

    processingChain.execute(testStrip, Processing::TNoteToLightMap());
    // strip is still zero
    EXPECT_EQ(strip, testStrip);
}

TEST_F(ProcessingChainTest, insertOne)
{
    processingChain.insertBlock(redSource);
    redSource = nullptr;

    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 10, 0, 0 };
    reference[1] = { 10, 0, 0 };
    reference[2] = { 10, 0, 0 };

    processingChain.execute(strip, Processing::TNoteToLightMap());
    EXPECT_EQ(reference, strip);
}

TEST_F(ProcessingChainTest, insertTwo)
{
    processingChain.insertBlock(redSource);
    redSource = nullptr;
    processingChain.insertBlock(valueDoubler);
    valueDoubler = nullptr;

    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 20, 0, 0 };
    reference[1] = { 20, 0, 0 };
    reference[2] = { 20, 0, 0 };

    processingChain.execute(strip, Processing::TNoteToLightMap());
    EXPECT_EQ(reference, strip);
}

TEST_F(ProcessingChainTest, convertToJson)
{
    Json::array mockBlocksJson;
    for(unsigned int i = 0; i < 3; ++i)
    {
        TMockBlock* mockBlock = new TMockBlock;
        ASSERT_NE(nullptr, mockBlock);

        Json mockJson = createMockBlockJson(i);
        EXPECT_CALL(*mockBlock, convertToJson())
            .WillOnce(Return(mockJson));
        mockBlocksJson.push_back(mockJson);

        processingChain.insertBlock(mockBlock);
    }

    Json::object converted = processingChain.convertToJson().object_items();
    EXPECT_EQ(mockBlocksJson, converted["processingChain"].array_items());
    EXPECT_EQ("ProcessingChain", converted.at("objectType").string_value());
}

TEST_F(ProcessingChainTest, convertFromJson)
{
    std::vector<IProcessingBlock*> mockBlocks({greenSource, valueDoubler});

    // Processing chain takes over ownership of the mock block when our mock factory returns it.
    // Prevent that the fixture teardown deletes already deleted object
    greenSource = nullptr;
    valueDoubler = nullptr;

    Json::array mockBlocksJson;
    for(unsigned int i = 0; i < mockBlocks.size(); ++i)
    {
        Json mockJson = createMockBlockJson(i);
        mockBlocksJson.push_back(mockJson);
        EXPECT_CALL(processingBlockFactory, createProcessingBlock(mockJson))
            .WillOnce(Return(mockBlocks[i]));
    }
    Json::object j;
    j["processingChain"] = mockBlocksJson;
    processingChain.convertFromJson(Json(j));

    Processing::TRgbStrip reference(3);
    reference[0] = {0, 20, 0};
    reference[1] = {0, 20, 0};
    reference[2] = {0, 20, 0};
    Processing::TRgbStrip testStrip(3);
    processingChain.execute(testStrip, Processing::TNoteToLightMap());
    EXPECT_EQ(reference, testStrip);
}

TEST_F(ProcessingChainTest, activate)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        processingChain.insertBlock(block);
        EXPECT_CALL(*block, activate());
        block = nullptr;
    }

    processingChain.activate();
}

TEST_F(ProcessingChainTest, deactivate)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        processingChain.insertBlock(block);
        EXPECT_CALL(*block, deactivate());
        block = nullptr;
    }

    processingChain.deactivate();
}

TEST_F(ProcessingChainTest, activateOnInsert)
{
    processingChain.activate();

    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        EXPECT_CALL(*block, activate());
        processingChain.insertBlock(block);
        block = nullptr;
    }
}

TEST_F(ProcessingChainTest, deactivateOnInsert)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        EXPECT_CALL(*block, deactivate());
        processingChain.insertBlock(block);
        block = nullptr;
    }
}
