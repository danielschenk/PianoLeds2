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
    for(auto byte : *pMessage)
    {
        processMidiByte(byte);
    }
}
