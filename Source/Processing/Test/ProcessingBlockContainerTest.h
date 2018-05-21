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

#ifndef PROCESSING_TEST_PROCESSINGBLOCKCONTAINERTEST_H_
#define PROCESSING_TEST_PROCESSINGBLOCKCONTAINERTEST_H_

#include <gtest/gtest.h>

#include <Common/Mock/LoggingTest.h>

#include "../Mock/MockProcessingBlock.h"
#include "../Mock/MockProcessingBlockFactory.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;

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

/**
 * Base fixture for processing block container tests.
 */
class ProcessingBlockContainerTest
    : public LoggingTest
{
public:
    static constexpr unsigned int c_stripSize = 3;

    // Make processing block mocks nice, to prevent warnings about unexpected calls to execute().
    typedef NiceMock<MockProcessingBlock> TMockBlock;

    ProcessingBlockContainerTest()
        : LoggingTest()
        , m_pRedSource(new TMockBlock())
        , m_pGreenSource(new TMockBlock())
        , m_pValueDoubler(new TMockBlock())
        , m_strip(c_stripSize)
        , m_processingBlockFactory()
    {
        ON_CALL(*m_pRedSource, execute(_))
            .WillByDefault(Invoke(addRed));
        ON_CALL(*m_pGreenSource, execute(_))
            .WillByDefault(Invoke(addGreen));
        ON_CALL(*m_pValueDoubler, execute(_))
            .WillByDefault(Invoke(doubleValue));
    }

    virtual ~ProcessingBlockContainerTest()
    {
        delete m_pRedSource;
        m_pRedSource = nullptr;
        delete m_pGreenSource;
        m_pGreenSource = nullptr;
        delete m_pValueDoubler;
        m_pValueDoubler = nullptr;
    }

    Json createMockBlockJson(unsigned int id)
    {
        Json::object j;
        j["id"] = static_cast<int>(id);

        return Json(j);
    }

    // These have to be manually allocated, as the container under test takes ownership and will try to delete it's children.
    TMockBlock* m_pRedSource;
    TMockBlock* m_pGreenSource;
    TMockBlock* m_pValueDoubler;

    Processing::TRgbStrip m_strip;

    MockProcessingBlockFactory m_processingBlockFactory;
};


#endif /* PROCESSING_TEST_PROCESSINGBLOCKCONTAINERTEST_H_ */
