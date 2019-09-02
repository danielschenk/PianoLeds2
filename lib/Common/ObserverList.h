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
 * @brief Template for observer lists.
 */

#ifndef COMMON_OBSERVERLIST_H_
#define COMMON_OBSERVERLIST_H_

#include <functional>
#include <vector>
#include <algorithm>

/**
 * Template for observer lists.
 */
template<class... CallbackArgs>
class ObserverList
{
public:
    /** Type for callback functions. */
    typedef std::function<void(CallbackArgs... args)> TCallbackFunction;

    /** Type for subscription tokens. */
    typedef unsigned int TSubscriptionToken;

    // Prevent implicit copy constructor and assignment operator.
    ObserverList(const ObserverList&) = delete;
    ObserverList& operator=(const ObserverList&) = delete;

    /**
     * Constructor.
     */
    ObserverList()
            : m_subscriptions()
    {
    }

    /**
     * Destructor.
     */
    virtual ~ObserverList() = default;

    /**
     * Subscribe for events.
     *
     * @param[in]   callback    The callback function to register.
     */
    TSubscriptionToken subscribe(TCallbackFunction callback)
    {
        // Check for a free slot in the vector first
        bool foundSlot = false;
        TSubscriptionToken token(-1);
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
            // No empty slot found, extend vector
            token = m_subscriptions.insert(m_subscriptions.cend(), callback) - m_subscriptions.cbegin();
        }

        return token;
    }

    /**
     * Unsubscribe from events.
     *
     * @param[in]  token   The token of the subscription to cancel.
     */
    void unsubscribe(TSubscriptionToken token)
    {
        if(token < m_subscriptions.size())
        {
            // Invalidate entry instead of removing it. This keeps previously handed out tokens valid.
            m_subscriptions[token] = nullptr;
        }
    }

    /**
     * Notify all subscribers.
     */
    void notifySubscribers(CallbackArgs... args) const
    {
        for(const auto& it : m_subscriptions)
        {
            if(it != nullptr)
            {
                it(args...);
            }
        }
    }

private:
    /** The collection of subscriptions. Using a vector for optimal traversal. */
    std::vector<TCallbackFunction> m_subscriptions;
};

#endif /* COMMON_OBSERVERLIST_H_ */
