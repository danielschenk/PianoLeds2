/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief
 */

#include <algorithm>

#include "Observable.h"

Observable::Observable()
    : m_subscriptions()
{
}

Observable::~Observable()
{
}

Observable::TSubscriptionToken Observable::subscribe(TCallbackFunction callback)
{
    // Check for a free slot in the vector first
    bool foundSlot = false;
    TSubscriptionToken token;
    for(auto it = m_subscriptions.begin(); it < m_subscriptions.end(); ++it)
    {
        if(*it == nullptr)
        {
            *it = callback;
            foundSlot = true;
            token = std::distance(m_subscriptions.begin(), it);
            break;
        }
    }

    if(!foundSlot)
    {
        token = m_subscriptions.insert(m_subscriptions.cend(), callback) - m_subscriptions.cbegin();
    }

    return token;
}

void Observable::unsubscribe(TSubscriptionToken token)
{
    if(token < m_subscriptions.size())
    {
        // Invalidate entry instead of removing it. This keeps previously handed out tokens valid.
        m_subscriptions[token] = nullptr;
    }
}

void Observable::notifySubscribers(TEventId eventId, const void* eventData) const
{
    for(const auto& it : m_subscriptions)
    {
        if(it != nullptr)
        {
            it(eventId, eventData);
        }
    }
}
