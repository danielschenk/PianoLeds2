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
