/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
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
