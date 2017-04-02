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

    /** Type of subscription token which can be used to unsubscribe. */
    typedef uint32_t TSubscriptionToken;

    /**
     * Subscribe to note receive events.
     *
     * @param   callback    [in]    The function to be called.
     */
    virtual TSubscriptionToken subscribeNoteOnOff(TNoteOnOffFunction callback) = 0;

    /**
     * Unsubscribe from note receive events.
     *
     * @param   token       [in]    The subscription to be removed.
     */
    virtual void unsubscribeNoteOnOff(TSubscriptionToken token) = 0;

    /**
     * Subscribe to control change events.
     *
     * @param   callback    [in]    The function to be called.
     */
    virtual TSubscriptionToken subscribeControlChange(TControlChangeFunction callback) = 0;

    /**
     * Unsubscribe from control change events.
     *
     * @param   token       [in]    The subscription to be removed.
     */
    virtual void unsubscribeControlChange(TSubscriptionToken token) = 0;
};



#endif /* SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_ */
