/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Base class for observable objects.
 */

#ifndef COMMON_OBSERVABLE_H_
#define COMMON_OBSERVABLE_H_

#include <functional>
#include <vector>

#include "Interfaces/IObservable.h"

/**
 * Base class for observable objects.
 */
class Observable
    : public IObservable
{
public:
    // Prevent implicit copy constructor and assignment operator.
    Observable(const Observable&) = delete;
    Observable& operator=(const Observable&) = delete;

    /**
     * Constructor.
     */
    Observable();

    /**
     * Destructor.
     */
    virtual ~Observable();

    // IObservable implementation
    virtual TSubscriptionToken subscribe(TCallbackFunction callback);
    virtual void unsubscribe(TSubscriptionToken token);

protected:
     /**
      * Notify all subscribers.
      */
     void notifySubscribers(TEventId eventId, const void* eventData) const;

private:
     /** The collection of subscriptions. Using a vector for optimal traversal. */
     std::vector<TCallbackFunction> m_subscriptions;
};

#endif /* COMMON_OBSERVABLE_H_ */
