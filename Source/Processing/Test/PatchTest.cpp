/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
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
 */

#include <gtest/gtest.h>

#include "ProcessingBlockContainerTest.h"
#include "../Patch.h"

using ::testing::Return;

class PatchTest
    : public ProcessingBlockContainerTest
{
public:
    PatchTest()
        : ProcessingBlockContainerTest()
        , m_patch(m_processingBlockFactory)
    {
    }

    virtual ~PatchTest()
    {
    }

    Patch m_patch;
};

TEST_F(PatchTest, defaults)
{
    EXPECT_EQ(0, m_patch.getBank());
    EXPECT_EQ(0, m_patch.getProgram());
    EXPECT_EQ(false, m_patch.hasBankAndProgram());
    EXPECT_EQ("Untitled Patch", m_patch.getName());
}

TEST_F(PatchTest, convertToJson)
{
    // Set some non-defaults
    m_patch.setBank(42);
    m_patch.setProgram(43);
    m_patch.setHasBankAndProgram(true);
    m_patch.setName("Awesome patch");

    std::vector<json> mockBlocksJson;
    for(unsigned int i = 0; i < 3; ++i)
    {
        TMockBlock* pMockBlock = new TMockBlock;
        ASSERT_NE(nullptr, pMockBlock);

        json mockJson = createMockBlockJson(i);
        EXPECT_CALL(*pMockBlock, convertToJson())
            .WillOnce(Return(mockJson));
        mockBlocksJson.push_back(mockJson);

        m_patch.insertBlock(pMockBlock);
    }

    json converted = m_patch.convertToJson();
    EXPECT_EQ(42, converted.at("bank").get<int>());
    EXPECT_EQ(43, converted.at("program").get<int>());
    EXPECT_EQ(true, converted.at("hasBankAndProgram").get<bool>());
    EXPECT_EQ("Awesome patch", converted.at("name").get<std::string>());

    EXPECT_EQ(mockBlocksJson, converted["processingChain"]);
    EXPECT_EQ("Patch", converted.at("objectType").get<std::string>());
}

TEST_F(PatchTest, convertFromJson)
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
    j["bank"] = 42;
    j["program"] = 43;
    j["hasBankAndProgram"] = true;
    j["name"] = std::string("Awesome patch");
    m_patch.convertFromJson(j);

    Processing::TRgbStrip reference(3);
    reference[0] = {0, 20, 0};
    reference[1] = {0, 20, 0};
    reference[2] = {0, 20, 0};
    Processing::TRgbStrip testStrip(3);
    m_patch.execute(testStrip);
    EXPECT_EQ(reference, testStrip);

    EXPECT_EQ(42, m_patch.getBank());
    EXPECT_EQ(43, m_patch.getProgram());
    EXPECT_EQ(true, m_patch.hasBankAndProgram());
    EXPECT_EQ("Awesome patch", m_patch.getName());
}
