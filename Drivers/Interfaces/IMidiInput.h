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

#include "IMidiInterface.h"

class IMidiInput
    : public IMidiInterface
{
public:
    /**
     * Interface to implement by MIDI input observers.
     */
    class IObserver
    {
    public:
        /**
         * Called when a note on/off message is received.
         *
         * @param channel       Channel number
         * @param pitch         Pitch (note number)
         * @param velocity      Velocity
         * @param on            True = note on, false = note off
         */
        virtual void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) = 0;

        /**
         * Called when a control change message is received.
         *
         * @param channel       Channel number
         * @param controller    Controller number
         * @param value         New value of the changed control
         */
        virtual void onControlChange(uint8_t channel, TControllerNumber controller, uint8_t value) = 0;

        /**
         * Called when a program change message is received.
         *
         * @param channel       Channel number
         * @param program       Number of the new active program
         */
        virtual void onProgramChange(uint8_t channel, uint8_t program) = 0;

        /**
         * Called when a channel pressure (a.k.a. after-touch) message is received.
         *
         * @param channel       Channel number
         * @param value         New channel pressure value
         */
        virtual void onChannelPressureChange(uint8_t channel, uint8_t value) = 0;

        /**
         * Called when a pitch bend message is received.
         *
         * @param channel       Channel number
         * @param value         New pitch bend value
         */
        virtual void onPitchBendChange(uint8_t channel, uint16_t value) = 0;

    protected:
        /**
         * Destructor.
         */
        ~IObserver() = default;
    };

    /**
     * Subscribe to MIDI events.
     *
     * @param   observer    The observer to subscribe
     */
    virtual void subscribe(IObserver& observer) = 0;

    /**
     * Unsubscribe from MIDI events.
     *
     * @param   observer    The observer to unsubscribe
     */
    virtual void unsubscribe(IObserver& observer) = 0;

protected:
    /**
     * Destructor.
     */
    virtual ~IMidiInput() = default;
};



#endif /* SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_ */
