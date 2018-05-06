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
