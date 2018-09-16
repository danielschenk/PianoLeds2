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
#include <list>
#include <vector>
#include <mutex>

#include "IMidiInput.h"

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
    virtual void subscribe(IObserver& observer);
    virtual void unsubscribe(IObserver& observer);

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
     * Notify observers about a note change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   pitch       The pitch of the note.
     * @param[in]   velocity    The velocity of the note.
     */
    void notifyNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const;

    /**
     * Notify observers about a control change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   controller  The number of the controller.
     * @param[in]   value       The value of the controller.
     */
    void notifyControlChange(uint8_t channel, IMidiInput::TControllerNumber control, uint8_t value) const;

    /**
     * Notify observers about a program change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   program     The program number.
     */
    void notifyProgramChange(uint8_t channel, uint8_t program) const;

    /**
     * Notify observers about a channel pressure (a.k.a. after-touch) change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   value       The new channel pressure value.
     */
    void notifyChannelPressureChange(uint8_t channel, uint8_t value) const;

    /**
     * Notify observers about a pitch bend change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   value       The new pitch bend value.
     */
    void notifyPitchBendChange(uint8_t channel, uint16_t value) const;

    /** Collection of observers. */
    std::list<IMidiInput::IObserver*> m_observers;

    /** Whether incoming bytes are stored to build a message. */
    bool m_buildingMessage;

    /** The message currently being built. */
    std::vector<uint8_t> m_currentMessage;

    /** Mutex to protect the observers. */
    mutable std::mutex m_observersMutex;
};

#endif /* DRIVERS_COMMON_BASEMIDIINPUT_H_ */
