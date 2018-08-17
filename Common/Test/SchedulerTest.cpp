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
 * @brief Unit test for the Scheduler class.
 */

#include <functional>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../Scheduler.h"

using ::testing::StrictMock;
using ::testing::Expectation;

class ITask
{
public:
    virtual void task() = 0;
};

class MockTask
    : public ITask
{
public:
    MOCK_METHOD0(task, void());
};

class SchedulerTest
    : public ::testing::Test
{
public:
    Scheduler m_scheduler;
};

TEST_F(SchedulerTest, executeOne)
{
    StrictMock<MockTask> mockTask;
    m_scheduler.schedule(std::bind(&MockTask::task, &mockTask));
    EXPECT_CALL(mockTask, task())
        .Times(1);

    EXPECT_TRUE(m_scheduler.executeOne());
    EXPECT_FALSE(m_scheduler.executeOne());
}

TEST_F(SchedulerTest, executeTwo)
{
    StrictMock<MockTask> mockTask1, mockTask2;
    m_scheduler.schedule(std::bind(&MockTask::task, &mockTask1));
    m_scheduler.schedule(std::bind(&MockTask::task, &mockTask2));

    Expectation task1call = EXPECT_CALL(mockTask1, task())
        .Times(1);
    EXPECT_CALL(mockTask2, task())
        .Times(1)
        .After(task1call);

    EXPECT_TRUE(m_scheduler.executeOne());
    EXPECT_TRUE(m_scheduler.executeOne());
    EXPECT_FALSE(m_scheduler.executeOne());
}

TEST_F(SchedulerTest, executeAll)
{
    StrictMock<MockTask> mockTask1, mockTask2;
    m_scheduler.schedule(std::bind(&MockTask::task, &mockTask1));
    m_scheduler.schedule(std::bind(&MockTask::task, &mockTask2));

    Expectation task1call = EXPECT_CALL(mockTask1, task())
        .Times(1);
    EXPECT_CALL(mockTask2, task())
        .Times(1)
        .After(task1call);

    EXPECT_TRUE(m_scheduler.executeAll());
    EXPECT_FALSE(m_scheduler.executeAll());
}
