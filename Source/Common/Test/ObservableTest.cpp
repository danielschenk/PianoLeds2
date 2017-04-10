/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Unit test for Observable.
 */

#include <functional>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../Observable.h"

using ::testing::StrictMock;
using ::testing::_;
using ::testing::Eq;
using ::testing::Pointee;

/** Need to derive from Observable to be able to test protected method */
class SomeObservable
    : public Observable
{
public:
    void event(Observable::TEventId eventId, const void* eventData)
    {
        notifySubscribers(eventId, eventData);
    }
};

class IObserver
{
public:
    virtual void callback(Observable::TEventId eventId, const void* eventData) = 0;
};

class MockObserver
    : public IObserver
{
public:
    MOCK_METHOD2(callback, void(Observable::TEventId, const void*));
};

class ObservableTest
    : public ::testing::Test
{
public:
    SomeObservable m_observable;
};

TEST_F(ObservableTest, subscribeOne)
{
    StrictMock<MockObserver> observer;
    m_observable.subscribe(std::bind(&MockObserver::callback, &observer, std::placeholders::_1, std::placeholders::_2));

    EXPECT_CALL(observer, callback(_, _))
        .Times(1);

    m_observable.event(42, nullptr);
}

TEST_F(ObservableTest, subscribeTwo)
{
    StrictMock<MockObserver> observer1, observer2;

    m_observable.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    m_observable.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));

    EXPECT_CALL(observer1, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer2, callback(_, _))
        .Times(1);

    m_observable.event(42, nullptr);
}

TEST_F(ObservableTest, unsubscribeFirst)
{
    StrictMock<MockObserver> observer1, observer2, observer3;

    Observable::TSubscriptionToken subscription1 = m_observable.subscribe(
            std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    m_observable.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));
    m_observable.subscribe(std::bind(&MockObserver::callback, &observer3, std::placeholders::_1, std::placeholders::_2));

    m_observable.unsubscribe(subscription1);

    EXPECT_CALL(observer2, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer3, callback(_, _))
        .Times(1);

    m_observable.event(42, nullptr);
}

TEST_F(ObservableTest, unsubscribeMiddle)
{
    StrictMock<MockObserver> observer1, observer2, observer3;

    m_observable.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    Observable::TSubscriptionToken subscription2 = m_observable.subscribe(
            std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));
    m_observable.subscribe(std::bind(&MockObserver::callback, &observer3, std::placeholders::_1, std::placeholders::_2));

    m_observable.unsubscribe(subscription2);

    EXPECT_CALL(observer1, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer3, callback(_, _))
        .Times(1);

    m_observable.event(42, nullptr);
}

TEST_F(ObservableTest, unsubscribeLast)
{
    StrictMock<MockObserver> observer1, observer2, observer3;

    m_observable.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    m_observable.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));
    Observable::TSubscriptionToken subscription3 = m_observable.subscribe(
            std::bind(&MockObserver::callback, &observer3, std::placeholders::_1, std::placeholders::_2));

    m_observable.unsubscribe(subscription3);

    EXPECT_CALL(observer1, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer2, callback(_, _))
        .Times(1);

    m_observable.event(42, nullptr);
}

TEST_F(ObservableTest, arguments)
{
    StrictMock<MockObserver> observer1, observer2;

    m_observable.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    m_observable.subscribe(std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));

    EXPECT_CALL(observer1, callback(42, nullptr))
        .Times(1);
    EXPECT_CALL(observer2, callback(42, nullptr))
        .Times(1);

    m_observable.event(42, nullptr);

    int aNumber = 42;
    EXPECT_CALL(observer1, callback(69, &aNumber))
        .Times(1);
    EXPECT_CALL(observer2, callback(69, &aNumber))
        .Times(1);

    m_observable.event(69, &aNumber);
}

TEST_F(ObservableTest, subscribeAfterUnsubscribe)
{
    StrictMock<MockObserver> observer1, observer2, observer3, observer4;

    m_observable.subscribe(std::bind(&MockObserver::callback, &observer1, std::placeholders::_1, std::placeholders::_2));
    Observable::TSubscriptionToken subscription2 = m_observable.subscribe(
            std::bind(&MockObserver::callback, &observer2, std::placeholders::_1, std::placeholders::_2));
    m_observable.subscribe(std::bind(&MockObserver::callback, &observer3, std::placeholders::_1, std::placeholders::_2));
    m_observable.subscribe(std::bind(&MockObserver::callback, &observer4, std::placeholders::_1, std::placeholders::_2));

    m_observable.unsubscribe(subscription2);

    EXPECT_CALL(observer1, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer3, callback(_, _))
        .Times(1);
    EXPECT_CALL(observer4, callback(_, _))
            .Times(1);

    m_observable.event(42, nullptr);
}
