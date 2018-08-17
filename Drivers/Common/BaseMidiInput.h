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
 * @brief Base class for MIDI inputs, implementing message subscription mechanism.
 */

#ifndef DRIVERS_COMMON_BASEMIDIINPUT_H_
#define DRIVERS_COMMON_BASEMIDIINPUT_H_

#include <cstdint>
#include <map>
#include <vector>
#include <Common/ObserverList.h>

#include "../Interfaces/IMidiInput.h"

/**
 * Base class for MIDI inputs, implementing message subscription mechanism.
 */
class BaseMidiInput
    : public IMidiInput
{
public:
    /**
     * Destructor.
     */
    virtual ~BaseMidiInput() = default;

    // Prevent implicit copy constructor and assignment operator.
    BaseMidiInput(const BaseMidiInput&) = delete;
    BaseMidiInput& operator=(const BaseMidiInput&) = delete;

    // IMidiInput implementation.
    virtual TSubscriptionToken subscribeNoteOnOff(TNoteOnOffFunction callback);
    virtual void unsubscribeNoteOnOff(TSubscriptionToken token);
    virtual TSubscriptionToken subscribeControlChange(TControlChangeFunction callback);
    virtual void unsubscribeControlChange(TSubscriptionToken token);
    virtual TSubscriptionToken subscribeProgramChange(TProgramChangeFunction callback);
    virtual void unsubscribeProgramChange(TSubscriptionToken token);

protected:
    /**
     * Constructor.
     */
    BaseMidiInput();

    /**
     * Process a single incoming MIDI byte.
     */
    void processMidiByte(uint8_t value);

private:
    /**
     * Notify all note on/off subscribers.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   pitch       The pitch of the note.
     * @param[in]   velocity    The pitch of the note.
     */
    virtual void notifyNoteOnOff(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const;

    /**
     * Notify all control change subscribers.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   controller  The number of the controller.
     * @param[in]   value       The value of the controller.
     */
    virtual void notifyControlChange(uint8_t channel, IMidiInput::TControllerNumber control, uint8_t value) const;

    /**
     * Notify all program change subscribers.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   number      The program number.
     */
    virtual void notifyProgramChange(uint8_t channel, uint8_t number) const;

    /** Collection of note on/off subscribers. */
    ObserverList<uint8_t, uint8_t, uint8_t, bool> m_noteOnOffSubscribers;

    /** Collection of control change subscribers. */
    ObserverList<uint8_t, IMidiInput::TControllerNumber, uint8_t> m_controlChangeSubscribers;

    /** Collection of program change subscribers. */
    ObserverList<uint8_t, uint8_t> m_programChangeSubscribers;

    /** Whether incoming bytes are stored to build a message. */
    bool m_buildingMessage;

    /** The message currently being built. */
    std::vector<uint8_t> m_currentMessage;
};

#endif /* DRIVERS_COMMON_BASEMIDIINPUT_H_ */
