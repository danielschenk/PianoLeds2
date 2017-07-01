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

    json createMockBlockJson(unsigned int id)
    {
        json j;
        j["id"] = id;

        return j;
    }

    // These have to be manually allocated, as the container under test takes ownership and will try to delete it's children.
    TMockBlock* m_pRedSource;
    TMockBlock* m_pGreenSource;
    TMockBlock* m_pValueDoubler;

    Processing::TRgbStrip m_strip;

    MockProcessingBlockFactory m_processingBlockFactory;
};


#endif /* PROCESSING_TEST_PROCESSINGBLOCKCONTAINERTEST_H_ */
