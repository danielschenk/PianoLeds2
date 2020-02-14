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
    typedef std::function<void(CallbackArgs... args)> TCallbackFunction;
    typedef unsigned int TSubscriptionToken;

    ObserverList() = default;

    ObserverList(const ObserverList&) = delete;
    ObserverList& operator=(const ObserverList&) = delete;

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
        for(auto it = subscriptions.begin(); it < subscriptions.end(); ++it)
        {
            if(*it == nullptr)
            {
                *it = callback;
                foundSlot = true;
                token = std::distance(subscriptions.begin(), it);
                break;
            }
        }

        if(!foundSlot)
        {
            // No empty slot found, extend vector
            token = subscriptions.insert(subscriptions.cend(), callback) - subscriptions.cbegin();
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
        if(token < subscriptions.size())
        {
            // Invalidate entry instead of removing it. This keeps previously handed out tokens valid.
            subscriptions[token] = nullptr;
        }
    }

    /**
     * Notify all observers.
     */
    void notifyObservers(CallbackArgs... args) const
    {
        for(const auto& it : subscriptions)
        {
            if(it != nullptr)
            {
                it(args...);
            }
        }
    }

private:
    /** The collection of subscriptions. Using a vector for optimal traversal. */
    std::vector<TCallbackFunction> subscriptions;
};

#endif /* COMMON_OBSERVERLIST_H_ */
