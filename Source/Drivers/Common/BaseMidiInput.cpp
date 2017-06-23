/**
 * @file
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
 * This file is part of MLC2.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
