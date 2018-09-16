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
#include "../ProcessingChain.h"
#include "../ProcessingTypes.h"

#define LOGGING_COMPONENT "ProcessingChain"

using ::testing::Return;
using ::testing::HasSubstr;

class ProcessingChainTest
    : public ProcessingBlockContainerTest
{
public:
    ProcessingChainTest()
        : m_processingChain(m_processingBlockFactory)
    {
    }

    virtual ~ProcessingChainTest()
    {
    }

    ProcessingChain m_processingChain;
};

TEST_F(ProcessingChainTest, empty)
{
    auto testStrip = Processing::TRgbStrip(c_stripSize);
    testStrip[0] = { 1, 0, 0 };
    testStrip[1] = { 0, 1, 0 };
    testStrip[2] = { 0, 0, 1 };

    m_processingChain.execute(testStrip, Processing::TNoteToLightMap());
    // m_strip is still zero
    EXPECT_EQ(m_strip, testStrip);
}

TEST_F(ProcessingChainTest, insertOne)
{
    m_processingChain.insertBlock(m_redSource);
    m_redSource = nullptr;

    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 10, 0, 0 };
    reference[1] = { 10, 0, 0 };
    reference[2] = { 10, 0, 0 };

    m_processingChain.execute(m_strip, Processing::TNoteToLightMap());
    EXPECT_EQ(reference, m_strip);
}

TEST_F(ProcessingChainTest, insertTwo)
{
    m_processingChain.insertBlock(m_redSource);
    m_redSource = nullptr;
    m_processingChain.insertBlock(m_valueDoubler);
    m_valueDoubler = nullptr;

    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 20, 0, 0 };
    reference[1] = { 20, 0, 0 };
    reference[2] = { 20, 0, 0 };

    m_processingChain.execute(m_strip, Processing::TNoteToLightMap());
    EXPECT_EQ(reference, m_strip);
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

        m_processingChain.insertBlock(mockBlock);
    }

    Json::object converted = m_processingChain.convertToJson().object_items();
    EXPECT_EQ(mockBlocksJson, converted["processingChain"].array_items());
    EXPECT_EQ("ProcessingChain", converted.at("objectType").string_value());
}

TEST_F(ProcessingChainTest, convertFromJson)
{
    std::vector<IProcessingBlock*> mockBlocks({m_greenSource, m_valueDoubler});

    // Processing chain takes over ownership of the mock block when our mock factory returns it.
    // Prevent that the fixture teardown deletes already deleted object
    m_greenSource = nullptr;
    m_valueDoubler = nullptr;

    Json::array mockBlocksJson;
    for(unsigned int i = 0; i < mockBlocks.size(); ++i)
    {
        Json mockJson = createMockBlockJson(i);
        mockBlocksJson.push_back(mockJson);
        EXPECT_CALL(m_processingBlockFactory, createProcessingBlock(mockJson))
            .WillOnce(Return(mockBlocks[i]));
    }
    Json::object j;
    j["processingChain"] = mockBlocksJson;
    m_processingChain.convertFromJson(Json(j));

    Processing::TRgbStrip reference(3);
    reference[0] = {0, 20, 0};
    reference[1] = {0, 20, 0};
    reference[2] = {0, 20, 0};
    Processing::TRgbStrip testStrip(3);
    m_processingChain.execute(testStrip, Processing::TNoteToLightMap());
    EXPECT_EQ(reference, testStrip);
}

TEST_F(ProcessingChainTest, activate)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        m_processingChain.insertBlock(block);
        EXPECT_CALL(*block, activate());
        block = nullptr;
    }

    m_processingChain.activate();
}

TEST_F(ProcessingChainTest, deactivate)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        m_processingChain.insertBlock(block);
        EXPECT_CALL(*block, deactivate());
        block = nullptr;
    }

    m_processingChain.deactivate();
}

TEST_F(ProcessingChainTest, activateOnInsert)
{
    m_processingChain.activate();

    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        EXPECT_CALL(*block, activate());
        m_processingChain.insertBlock(block);
        block = nullptr;
    }
}

TEST_F(ProcessingChainTest, deactivateOnInsert)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        EXPECT_CALL(*block, deactivate());
        m_processingChain.insertBlock(block);
        block = nullptr;
    }
}
