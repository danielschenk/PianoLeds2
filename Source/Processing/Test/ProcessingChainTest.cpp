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

#include <Common/Mock/LoggingTest.h>

#include "ProcessingBlockContainerTest.h"
#include "../ProcessingChain.h"
#include "../Interfaces/ProcessingTypes.h"

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
    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 1, 0, 0 };
    reference[1] = { 0, 1, 0 };
    reference[2] = { 0, 0, 1 };
    m_strip = reference;

    m_processingChain.execute(m_strip);
    EXPECT_EQ(reference, m_strip);
}

TEST_F(ProcessingChainTest, insertOne)
{
    m_processingChain.insertBlock(m_pRedSource);
    m_pRedSource = nullptr;

    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 10, 0, 0 };
    reference[1] = { 10, 0, 0 };
    reference[2] = { 10, 0, 0 };

    m_processingChain.execute(m_strip);
    EXPECT_EQ(reference, m_strip);
}

TEST_F(ProcessingChainTest, insertTwo)
{
    m_processingChain.insertBlock(m_pRedSource);
    m_pRedSource = nullptr;
    m_processingChain.insertBlock(m_pValueDoubler);
    m_pValueDoubler = nullptr;

    auto reference = Processing::TRgbStrip(c_stripSize);
    reference[0] = { 20, 0, 0 };
    reference[1] = { 20, 0, 0 };
    reference[2] = { 20, 0, 0 };

    m_processingChain.execute(m_strip);
    EXPECT_EQ(reference, m_strip);
}

TEST_F(ProcessingChainTest, convertToJson)
{
    std::vector<json> mockBlocksJson;
    for(unsigned int i = 0; i < 3; ++i)
    {
        TMockBlock* pMockBlock = new TMockBlock;
        ASSERT_NE(nullptr, pMockBlock);

        json mockJson = createMockBlockJson(i);
        EXPECT_CALL(*pMockBlock, convertToJson())
            .WillOnce(Return(mockJson));
        mockBlocksJson.push_back(mockJson);

        m_processingChain.insertBlock(pMockBlock);
    }

    json converted = m_processingChain.convertToJson();
    EXPECT_EQ(mockBlocksJson, converted["processingChain"]);
    EXPECT_EQ("ProcessingChain", converted.at("objectType").get<std::string>());
}

TEST_F(ProcessingChainTest, convertFromJson)
{
    std::vector<IProcessingBlock*> mockBlocks({m_pGreenSource, m_pValueDoubler});

    // Processing chain takes over ownership of the mock block when our mock factory returns it.
    // Prevent that the fixture teardown deletes already deleted object
    m_pGreenSource = nullptr;
    m_pValueDoubler = nullptr;

    std::vector<json> mockBlocksJson;
    for(unsigned int i = 0; i < mockBlocks.size(); ++i)
    {
        json mockJson = createMockBlockJson(i);
        mockBlocksJson.push_back(mockJson);
        EXPECT_CALL(m_processingBlockFactory, createProcessingBlock(mockJson))
            .WillOnce(Return(mockBlocks[i]));
    }
    json j;
    j["processingChain"] = mockBlocksJson;
    m_processingChain.convertFromJson(j);

    Processing::TRgbStrip reference(3);
    reference[0] = {0, 20, 0};
    reference[1] = {0, 20, 0};
    reference[2] = {0, 20, 0};
    Processing::TRgbStrip testStrip(3);
    m_processingChain.execute(testStrip);
    EXPECT_EQ(reference, testStrip);
}

TEST_F(ProcessingChainTest, convertFromJsonMissingChain)
{
    json j;
    EXPECT_CALL(m_mockLoggingTarget, logMessage(_, Logging::LogLevel_Error, LOGGING_COMPONENT, _));
    m_processingChain.convertFromJson(j);
}

TEST_F(ProcessingChainTest, convertFromJsonUnrecognizedField)
{
    json mockJson = createMockBlockJson(0);
    EXPECT_CALL(m_processingBlockFactory, createProcessingBlock(mockJson))
        .WillOnce(Return(m_pGreenSource));
    // Processing chain takes over ownership of the mock block when our mock factory returns it.
    // Prevent that the fixture teardown deletes already deleted object
    m_pGreenSource = nullptr;

    std::vector<json> mockBlocksJson;
    mockBlocksJson.push_back(mockJson);
    json j;
    j["processingChain"] = mockBlocksJson;
    j["futureField"] = 42;
    j["futureField2"] = 43;

    EXPECT_CALL(m_mockLoggingTarget, logMessage(_, Logging::LogLevel_Warning, LOGGING_COMPONENT, HasSubstr("futureField")));
    EXPECT_CALL(m_mockLoggingTarget, logMessage(_, Logging::LogLevel_Warning, LOGGING_COMPONENT, HasSubstr("futureField2")));
    m_processingChain.convertFromJson(j);
}

TEST_F(ProcessingChainTest, activate)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* pBlock = new TMockBlock;
        m_processingChain.insertBlock(pBlock);
        EXPECT_CALL(*pBlock, activate());
        pBlock = nullptr;
    }

    m_processingChain.activate();
}

TEST_F(ProcessingChainTest, deactivate)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* pBlock = new TMockBlock;
        m_processingChain.insertBlock(pBlock);
        EXPECT_CALL(*pBlock, deactivate());
        pBlock = nullptr;
    }

    m_processingChain.deactivate();
}

TEST_F(ProcessingChainTest, activateOnInsert)
{
    m_processingChain.activate();

    for(int i = 0; i < 3; i++)
    {
        TMockBlock* pBlock = new TMockBlock;
        EXPECT_CALL(*pBlock, activate());
        m_processingChain.insertBlock(pBlock);
        pBlock = nullptr;
    }
}

TEST_F(ProcessingChainTest, deactivateOnInsert)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* pBlock = new TMockBlock;
        EXPECT_CALL(*pBlock, deactivate());
        m_processingChain.insertBlock(pBlock);
        pBlock = nullptr;
    }
}
