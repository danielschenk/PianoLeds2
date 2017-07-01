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
