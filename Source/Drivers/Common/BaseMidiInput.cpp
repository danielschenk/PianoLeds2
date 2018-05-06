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

BaseMidiInput::BaseMidiInput()
    : m_noteOnOffSubscribers()
    , m_controlChangeSubscribers()
{
}

BaseMidiInput::~BaseMidiInput()
{
}

IMidiInput::TSubscriptionToken BaseMidiInput::subscribeNoteOnOff(IMidiInput::TNoteOnOffFunction callback)
{
    return m_noteOnOffSubscribers.subscribe(callback);
}

void BaseMidiInput::unsubscribeNoteOnOff(IMidiInput::TSubscriptionToken token)
{
    m_noteOnOffSubscribers.unsubscribe(token);
}

IMidiInput::TSubscriptionToken BaseMidiInput::subscribeControlChange(IMidiInput::TControlChangeFunction callback)
{
    return m_controlChangeSubscribers.subscribe(callback);
}

void BaseMidiInput::unsubscribeControlChange(IMidiInput::TSubscriptionToken token)
{
    m_controlChangeSubscribers.unsubscribe(token);
}

void BaseMidiInput::notifyNoteOnOff(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const
{
    m_noteOnOffSubscribers.notifySubscribers(channel, pitch, velocity, on);
}

IMidiInput::TSubscriptionToken BaseMidiInput::subscribeProgramChange(TProgramChangeFunction callback)
{
    return m_programChangeSubscribers.subscribe(callback);
}

void BaseMidiInput::unsubscribeProgramChange(TSubscriptionToken token)
{
    m_programChangeSubscribers.unsubscribe(token);
}

void BaseMidiInput::notifyControlChange(uint8_t channel, IMidiInterface::TControllerNumber controller, uint8_t value) const
{
    m_controlChangeSubscribers.notifySubscribers(channel, controller, value);
}

void BaseMidiInput::notifyProgramChange(uint8_t channel, uint8_t number) const
{
    m_programChangeSubscribers.notifySubscribers(channel, number);
}
