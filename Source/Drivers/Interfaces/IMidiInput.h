/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of mlc2.
 * 
 * @brief Interface for MIDI inputs.
 */

#ifndef SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_
#define SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_

#include <cstdint>
#include <functional>

#include "IMidiInterface.h"

class IMidiInput
    : public IMidiInterface
{
public:
    /** Function type for note on and off events. Arguments: channel, note number, velocity, on/off. */
    typedef std::function<void(uint8_t, uint8_t, uint8_t, bool)> TNoteOnOffFunction;

    /** Function type for control change events. Arguments: channel, controller number, value. */
    typedef std::function<void(uint8_t, TControllerNumber, uint8_t)> TControlChangeFunction;

    /** Function type for program change events. Arguments: channel, number. */
    typedef std::function<void(uint8_t, uint8_t)> TProgramChangeFunction;

    /** Type of subscription token which can be used to unsubscribe. */
    typedef unsigned int TSubscriptionToken;

    /**
     * Destructor.
     */
    virtual ~IMidiInput()
    {
    };

    /**
     * Subscribe to note receive events.
     *
     * @param[in]   callback    The function to be called.
     */
    virtual TSubscriptionToken subscribeNoteOnOff(TNoteOnOffFunction callback) = 0;

    /**
     * Unsubscribe from note receive events.
     *
     * @param[in]   token       The subscription to be removed.
     */
    virtual void unsubscribeNoteOnOff(TSubscriptionToken token) = 0;

    /**
     * Subscribe to control change events.
     *
     * @param[in]   callback    The function to be called.
     */
    virtual TSubscriptionToken subscribeControlChange(TControlChangeFunction callback) = 0;

    /**
     * Unsubscribe from control change events.
     *
     * @param[in]   token       The subscription to be removed.
     */
    virtual void unsubscribeControlChange(TSubscriptionToken token) = 0;

    /**
     * Subscribe to program change events.
     *
     * @param[in]   callback    The function to be called.
     */
    virtual TSubscriptionToken subscribeProgramChange(TProgramChangeFunction callback) = 0;

    /**
     * Unsubscribe from program change events.
     *
     * @param[in]   token       The subscription to be removed.
     */
    virtual void unsubscribeProgramChange(TSubscriptionToken token) = 0;
};



#endif /* SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_ */
