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
    virtual ~ObserverList()
    {
    }

    /**
     * Subscribe for events.
     *
     * @param[in]   callback    The callback function to register.
     */
     TSubscriptionToken subscribe(TCallbackFunction callback)
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
