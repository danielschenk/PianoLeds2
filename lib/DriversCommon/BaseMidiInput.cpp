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
 */

#include "BaseMidiInput.h"

#include <Logging.h>

#define LOGGING_COMPONENT "BaseMidiInput"

BaseMidiInput::BaseMidiInput()
    : m_observers()
    , m_buildingMessage(false)
    , m_currentMessage()
    , m_observersMutex()
{
}

void BaseMidiInput::subscribe(IObserver& observer)
{
    std::lock_guard<std::mutex> lock(m_observersMutex);
    m_observers.push_back(&observer);
}

void BaseMidiInput::unsubscribe(IObserver& observer)
{
    std::lock_guard<std::mutex> lock(m_observersMutex);
    m_observers.remove(&observer);
}

void BaseMidiInput::notifyNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const
{
    std::lock_guard<std::mutex> lock(m_observersMutex);

    for(auto observer : m_observers)
    {
        observer->onNoteChange(channel, pitch, velocity, on);
    }
}

void BaseMidiInput::notifyControlChange(uint8_t channel, IMidiInterface::TControllerNumber controller, uint8_t value) const
{
    std::lock_guard<std::mutex> lock(m_observersMutex);

    for(auto observer : m_observers)
    {
        observer->onControlChange(channel, controller, value);
    }
}

void BaseMidiInput::notifyProgramChange(uint8_t channel, uint8_t program) const
{
    std::lock_guard<std::mutex> lock(m_observersMutex);

    for(auto observer : m_observers)
    {
        observer->onProgramChange(channel, program);
    }
}

void BaseMidiInput::notifyChannelPressureChange(uint8_t channel, uint8_t value) const
{
    std::lock_guard<std::mutex> lock(m_observersMutex);

    for(auto observer : m_observers)
    {
        observer->onChannelPressureChange(channel, value);
    }
}

void BaseMidiInput::notifyPitchBendChange(uint8_t channel, uint16_t value) const
{
    std::lock_guard<std::mutex> lock(m_observersMutex);

    for(auto observer : m_observers)
    {
        observer->onPitchBendChange(channel, value);
    }
}

void BaseMidiInput::processMidiByte(uint8_t value)
{
    if(!m_buildingMessage && ((value & 0x80) == 0x80))
    {
        // Is a status byte. Start building new message
        m_currentMessage.clear();
        m_buildingMessage = true;
    }

    if(m_buildingMessage)
    {
        m_currentMessage.push_back(value);

        // Get status (high nibble) and channel (low nibble) from status byte
        uint8_t statusByte(m_currentMessage[0]);
        uint8_t status(statusByte & 0xF0);
        uint8_t channel(statusByte & 0x0F);

        // Check if a message can be parsed and sent to subscribers.
        switch(static_cast<IMidiInterface::TStatus>(status))
        {
        case NOTE_OFF:
            if(m_currentMessage.size() >= 3)
            {
                // Channel, pitch, velocity, note off
                notifyNoteChange(channel, m_currentMessage[1], m_currentMessage[2], false);
                m_buildingMessage = false;
            }
            break;

        case NOTE_ON:
            if(m_currentMessage.size() >= 3)
            {
                // Channel, pitch, velocity, note on
                notifyNoteChange(channel, m_currentMessage[1], m_currentMessage[2], true);
                m_buildingMessage = false;
            }
            break;

        case CONTROL_CHANGE:
            if(m_currentMessage.size() >= 3)
            {
                // Channel, controller number, value
                notifyControlChange(channel, (IMidiInterface::TControllerNumber)m_currentMessage[1], m_currentMessage[2]);
                m_buildingMessage = false;
            }
            break;

        case PROGRAM_CHANGE:
            if(m_currentMessage.size() >= 2)
            {
                // Channel, number
                notifyProgramChange(channel, m_currentMessage[1]);
                m_buildingMessage = false;
            }
            break;

        case CHANNEL_PRESSURE_CHANGE:
            if(m_currentMessage.size() >= 2)
            {
                // Channel, value
                notifyChannelPressureChange(channel, m_currentMessage[1]);
                m_buildingMessage = false;
            }
            break;

        case PITCH_BEND_CHANGE:
            if(m_currentMessage.size() >= 3)
            {
                // Pitch bend value is a 14-bit value.
                // The first byte contains the low 7 bits, the second byte the high 7 bits.
                uint16_t value(m_currentMessage[1] | (m_currentMessage[2] << 7));

                // Channel, value
                notifyPitchBendChange(channel, value);
                m_buildingMessage = false;
            }
            break;

        default:
            // Unsupported status.
            LOG_WARNING_PARAMS("Unsupported MIDI status %#02x on channel %2u, ignoring rest of message.", status, channel);
            m_buildingMessage = false;
            break;
        }
    }
}
