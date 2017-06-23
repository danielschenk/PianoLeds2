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
 * @brief Unit test for the ObserverList template.
 */

#include <Common/ObserverList.h>
#include <functional>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


using ::testing::StrictMock;
using ::testing::_;

class IObserver
{
public:
    virtual void callback(int arg1, const void* arg2) = 0;
};

class MockObserver
    : public IObserver
{
public:
    MOCK_METHOD2(callback, void(int, const void*));
};

class ObserverListTest
    : public ::testing::Test
{
public:
    ObserverList<int, const void*> m_observerList;
};

TEST_F(ObserverListTest, subscribeOne)
{
    StrictMock<MockObserver> observer;
    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer, std::placeholders::_1, std::placeholders::_2));

    EXPECT_CALL(observer, callback(_, _))
        .Times(1);

    m_observerList.notifySubscribers(42, nullptr);
}

TEST_F(ObserverListTest, subscribeTwo)
{
    StrictMock<MockObserver> observer1, observer2;

    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));

    EXPECT_CALL(observer1, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer2, callback(_, _))
        .Times(1);

    m_observerList.notifySubscribers(42, nullptr);
}

TEST_F(ObserverListTest, unsubscribeFirst)
{
    StrictMock<MockObserver> observer1, observer2, observer3;

    auto subscription1 = m_observerList.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));
    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer3, std::placeholders::_1, std::placeholders::_2));

    m_observerList.unsubscribe(subscription1);

    EXPECT_CALL(observer2, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer3, callback(_, _))
        .Times(1);

    m_observerList.notifySubscribers(42, nullptr);
}

TEST_F(ObserverListTest, unsubscribeMiddle)
{
    StrictMock<MockObserver> observer1, observer2, observer3;

    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    auto subscription2 = m_observerList.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));
    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer3, std::placeholders::_1, std::placeholders::_2));

    m_observerList.unsubscribe(subscription2);

    EXPECT_CALL(observer1, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer3, callback(_, _))
        .Times(1);

    m_observerList.notifySubscribers(42, nullptr);
}

TEST_F(ObserverListTest, unsubscribeLast)
{
    StrictMock<MockObserver> observer1, observer2, observer3;

    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));
    auto subscription3 = m_observerList.subscribe(std::bind(&MockObserver::callback, &observer3, std::placeholders::_1, std::placeholders::_2));

    m_observerList.unsubscribe(subscription3);

    EXPECT_CALL(observer1, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer2, callback(_, _))
        .Times(1);

    m_observerList.notifySubscribers(42, nullptr);
}

TEST_F(ObserverListTest, arguments)
{
    StrictMock<MockObserver> observer1, observer2;

    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));

    EXPECT_CALL(observer1, callback(42, nullptr))
        .Times(1);
    EXPECT_CALL(observer2, callback(42, nullptr))
        .Times(1);

    m_observerList.notifySubscribers(42, nullptr);

    int aNumber = 42;
    EXPECT_CALL(observer1, callback(69, &aNumber))
        .Times(1);
    EXPECT_CALL(observer2, callback(69, &aNumber))
        .Times(1);

    m_observerList.notifySubscribers(69, &aNumber);
}

TEST_F(ObserverListTest, subscribeAfterUnsubscribe)
{
    StrictMock<MockObserver> observer1, observer2, observer3, observer4;

    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    auto subscription2 = m_observerList.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));
    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer3, std::placeholders::_1, std::placeholders::_2));

    m_observerList.unsubscribe(subscription2);

    m_observerList.subscribe(std::bind(&MockObserver::callback, &observer4, std::placeholders::_1, std::placeholders::_2));

    EXPECT_CALL(observer1, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer3, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer4, callback(_, _))
        .Times(1);

    m_observerList.notifySubscribers(42, nullptr);
}

TEST_F(ObserverListTest, unsubscribeInvalidToken)
{
    StrictMock<MockObserver> observer1, observer2, observer3;

    auto subscription1 = m_observerList.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    auto subscription2 = m_observerList.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));
    auto subscription3 = m_observerList.subscribe(std::bind(&MockObserver::callback, &observer3, std::placeholders::_1, std::placeholders::_2));

    // Don't assume anything about the way tokens are generated, make sure we find an unused one
    unsigned int invalidToken = 0;
    bool found = false;
    for(unsigned int i = 0; i < 1000; ++i)
    {
        if(i != subscription1 && i != subscription2 && i != subscription3)
        {
            invalidToken = i;
            found = true;
            break;
        }
    }
    ASSERT_EQ(true, found);

    m_observerList.unsubscribe(invalidToken);

    EXPECT_CALL(observer1, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer2, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer3, callback(_, _))
        .Times(1);

    m_observerList.notifySubscribers(42, nullptr);
}
