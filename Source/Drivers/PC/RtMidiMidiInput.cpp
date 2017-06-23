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

#include <cstdio>
#include <cassert>
#include <rtmidi/RtMidi.h>

#include "RtMidiMidiInput.h"

RtMidiMidiInput::RtMidiMidiInput()
    : m_pRtMidiIn(new RtMidiIn())
{
    assert(m_pRtMidiIn != nullptr);
    m_pRtMidiIn->setCallback(&RtMidiCommonCallback, (void*)this);
}

RtMidiMidiInput::~RtMidiMidiInput()
{
    delete m_pRtMidiIn;
}

unsigned int RtMidiMidiInput::getPortCount() const
{
    return m_pRtMidiIn->getPortCount();
}

void RtMidiMidiInput::openPort(int number)
{
    m_pRtMidiIn->openPort(number);
}

void RtMidiMidiInput::RtMidiCommonCallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
    // userData tells us the instance to call the specific callback on
    RtMidiMidiInput* pMidiInput = static_cast<RtMidiMidiInput*>(userData);
    if(pMidiInput != nullptr)
    {
        pMidiInput->RtMidiCallback(deltatime, message);
    }
}

void RtMidiMidiInput::RtMidiCallback(double deltatime, std::vector<unsigned char> *pMessage)
{
    // Get status (high nibble) and channel (low nibble) from status byte
    uint8_t status = pMessage->at(0) & 0xF0;
    uint8_t channel = pMessage->at(0) & 0x0F;

    switch(status)
    {
        case NOTE_OFF:
            // Channel, pitch, velocity, note off
            notifyNoteOnOff(channel, pMessage->at(1), pMessage->at(2), false);
            break;
        case NOTE_ON:
            // Channel, pitch, velocity, note on
            notifyNoteOnOff(channel, pMessage->at(1), pMessage->at(2), true);
            break;
        case CONTROL_CHANGE:
            // Channel, controller number, value
            notifyControlChange(channel, (IMidiInterface::TControllerNumber)pMessage->at(1), pMessage->at(2));
            break;
        case PROGRAM_CHANGE:
            // Channel, number
            notifyProgramChange(channel, pMessage->at(1));
            break;
        default:
            break;
    }
}
