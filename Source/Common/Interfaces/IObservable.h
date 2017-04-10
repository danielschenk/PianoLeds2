/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Interface for observables.
 */

#ifndef COMMON_INTERFACES_IOBSERVABLE_H_
#define COMMON_INTERFACES_IOBSERVABLE_H_

/**
 * Interface for observables.
 */
class IObservable
{
public:
    /** Type for event IDs. */
    typedef unsigned int TEventId;

    /** Type for callback functions. Arguments: event ID, pointer to event data. */
    typedef std::function<void(TEventId, const void*)> TCallbackFunction;

    /** Type for subscription tokens. */
    typedef unsigned int TSubscriptionToken;

    /**
     * Subscribe for events.
     *
     * @param   [in]    The callback function to register.
     */
     virtual TSubscriptionToken subscribe(TCallbackFunction callback) = 0;

     /**
      * Unsubscribe from events.
      *
      * @param  [in]    The token of the subscription to cancel.
      */
     virtual void unsubscribe(TSubscriptionToken token) = 0;
};


#endif /* COMMON_INTERFACES_IOBSERVABLE_H_ */
