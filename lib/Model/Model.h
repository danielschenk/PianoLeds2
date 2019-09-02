/**
 * @file
 *
 * MIT License
 *
 * @copyright (c) 2019 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 */

#ifndef MODEL_H
#define MODEL_H

#include "ObserverList.h"

#include <mutex>

/**
 * Base class for observable objects
 */
class Model
{
public:
    /**
     * Constructor
     */
    Model() = default;

    /**
     * Destructor
     */
    virtual ~Model() = default;

    // Prevent implicit copy constructor & assignment operator
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    typedef std::function<void()> TUpdateCallback;
    typedef ObserverList<> UpdateObserverList;

    /**
     * Subscribe to model updates.
     *
     * @param callback  The callback to call on a model update
     * @return          The token which can be used to unsubscribe
     */
    UpdateObserverList::TSubscriptionToken subscribe(TUpdateCallback callback);

    /**
     * Unsubscribe from model updates.
     *
     * @param token     The token received for the subscription
     */
    void unsubscribe(UpdateObserverList::TSubscriptionToken token);

protected:
    /**
     * Set a value and notify observers.
     *
     * @param member    Reference to the member to set
     * @param value     Value to set
     */
    template<typename T>
    void set(T& member, T value)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            member = value;
        }

        // Outside data lock, to prevent deadlock when observer calls a getter
        notifyObservers();
    }

    /**
     * Get a value.
     *
     * @param member    Reference to the member to get
     */
    template<typename T>
    T get(const T& member) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return member;
    }

private:
    /**
     * Notify all observers about a model update.
     */
    void notifyObservers() const;

    /**
     * List of observers
     */
    UpdateObserverList m_observers;

    /**
     * Mutex guarding the observers
     */
    mutable std::mutex m_observersMutex;

    /**
     * Mutex guarding the data
     */
     mutable std::mutex m_mutex;
};


#endif //MODEL_H
