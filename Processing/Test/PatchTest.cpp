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
        : m_processingBlockFactory()
        , m_pProcessingChain(new NiceMock<MockProcessingChain>)
    {
        ON_CALL(m_processingBlockFactory, createProcessingChain())
            .WillByDefault(Return(m_pProcessingChain));

        m_pPatch = new Patch(m_processingBlockFactory);
    }

    virtual ~PatchTest()
    {
        delete m_pPatch;

        // deleted by Patch
        m_pProcessingChain = nullptr;
    }

    NiceMock<MockProcessingBlockFactory> m_processingBlockFactory;
    NiceMock<MockProcessingChain>* m_pProcessingChain;
    Patch* m_pPatch;
};

TEST_F(PatchTest, defaults)
{
    EXPECT_EQ(0, m_pPatch->getBank());
    EXPECT_EQ(0, m_pPatch->getProgram());
    EXPECT_EQ(false, m_pPatch->hasBankAndProgram());
    EXPECT_EQ("Untitled Patch", m_pPatch->getName());
}

TEST_F(PatchTest, convertToJson)
{
    // Set some non-defaults
    m_pPatch->setBank(42);
    m_pPatch->setProgram(43);
    m_pPatch->setName("Awesome patch");

    Json::object mockChainJson;
    mockChainJson["objectType"] = "mockChain";
    mockChainJson["someProperty"] = 42;
    EXPECT_CALL(*m_pProcessingChain, convertToJson())
        .WillOnce(Return(mockChainJson));

    Json::object converted = m_pPatch->convertToJson().object_items();
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

    EXPECT_CALL(*m_pProcessingChain, convertFromJson(Json(mockChainJson)));
    m_pPatch->convertFromJson(Json(j));

    EXPECT_EQ(42, m_pPatch->getBank());
    EXPECT_EQ(43, m_pPatch->getProgram());
    EXPECT_EQ(true, m_pPatch->hasBankAndProgram());
    EXPECT_EQ("Awesome patch", m_pPatch->getName());
}

TEST_F(PatchTest, activate)
{
    EXPECT_CALL(*m_pProcessingChain, activate());
    m_pPatch->activate();
}

TEST_F(PatchTest, deactivate)
{
    EXPECT_CALL(*m_pProcessingChain, deactivate());
    m_pPatch->deactivate();
}

TEST_F(PatchTest, execute)
{
    Processing::TRgbStrip strip;
    strip.push_back(Processing::TRgb({0, 0, 0}));

    // Let the mock processing chain do something with the strip which we can verify
    Processing::TRgb valueAfterProcessing({1, 2, 3});
    ASSERT_NE(valueAfterProcessing, strip[0]);

    EXPECT_CALL(*m_pProcessingChain, execute(_))
        .WillOnce(Invoke([valueAfterProcessing](Processing::TRgbStrip& rStrip){
            rStrip[0] = valueAfterProcessing;
    }));
    m_pPatch->execute(strip);

    EXPECT_EQ(valueAfterProcessing, strip[0]);
}
