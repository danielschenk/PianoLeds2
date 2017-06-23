/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Unit tests for ProcessingBlock.
 */

#include <string>
#include <vector>
#include <gtest/gtest.h>

#include <Common/Mock/LoggingTest.h>

#include "../Mock/MockProcessingBlock.h"
#include "../Mock/MockProcessingBlockFactory.h"
#include "../ProcessingChain.h"
#include "../Interfaces/ProcessingTypes.h"

#define LOGGING_COMPONENT "ProcessingChain"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::HasSubstr;

static void addRed(Processing::TRgbStrip& strip)
{
    for(auto& rLed : strip)
    {
        rLed.r = 10;
    }
}

static void addGreen(Processing::TRgbStrip& strip)
{
    for(auto& rLed : strip)
    {
        rLed.g = 10;
    }
}

static void doubleValue(Processing::TRgbStrip& strip)
{
    for(auto& rLed : strip)
    {
        rLed.r = std::min(0xff, rLed.r*2);
        rLed.g = std::min(0xff, rLed.g*2);
        rLed.b = std::min(0xff, rLed.b*2);
    }
}

class ProcessingChainTest
    : public LoggingTest
{
public:
    static constexpr unsigned int c_stripSize = 3;

    // Make processing block mocks nice, to prevent warnings about unexpected calls to execute().
    typedef NiceMock<MockProcessingBlock> TMockBlock;

    ProcessingChainTest()
        : LoggingTest()
        , m_pRedSource(new TMockBlock())
        , m_pGreenSource(new TMockBlock())
        , m_pValueDoubler(new TMockBlock())
        , m_strip(c_stripSize)
        , m_processingBlockFactory()
        , m_processingChain(m_processingBlockFactory)
    {
        ON_CALL(*m_pRedSource, execute(_))
            .WillByDefault(Invoke(addRed));
        ON_CALL(*m_pGreenSource, execute(_))
            .WillByDefault(Invoke(addGreen));
        ON_CALL(*m_pValueDoubler, execute(_))
            .WillByDefault(Invoke(doubleValue));
    }

    virtual ~ProcessingChainTest()
    {
        delete m_pRedSource;
        m_pRedSource = nullptr;
        delete m_pGreenSource;
        m_pGreenSource = nullptr;
        delete m_pValueDoubler;
        m_pValueDoubler = nullptr;
    }

    json createMockBlockJson(unsigned int id)
    {
        json j;
        j["id"] = id;

        return j;
    }

    // These have to be pointers, as the processing chain takes ownership and will try to delete it's children.
    TMockBlock* m_pRedSource;
    TMockBlock* m_pGreenSource;
    TMockBlock* m_pValueDoubler;

    Processing::TRgbStrip m_strip;

    MockProcessingBlockFactory m_processingBlockFactory;

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
