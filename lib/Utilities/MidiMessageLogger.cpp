/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2018 Daniel Schenk <danielschenk@users.noreply.github.com>
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

#include "Logging.h"

#include "MidiMessageLogger.h"

#define LOGGING_COMPONENT "MidiMessageLogger"

MidiMessageLogger::MidiMessageLogger(IMidiInput& midiInput)
    : m_midiInput(midiInput)
{
    m_midiInput.subscribe(*this);
}

MidiMessageLogger::~MidiMessageLogger()
{
    m_midiInput.unsubscribe(*this);
}

void MidiMessageLogger::onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on)
{
    LOG_INFO_PARAMS("%3s chan %2u pitch %3u vel %3u", on ? "ON" : "OFF", channel, pitch, velocity);
}

void MidiMessageLogger::onControlChange(uint8_t channel, IMidiInterface::TControllerNumber controller, uint8_t value)
{
    LOG_INFO_PARAMS("CON chan %2u controller %3u val %3u", channel, controller, value);
}

void MidiMessageLogger::onProgramChange(uint8_t channel, uint8_t program)
{
    LOG_INFO_PARAMS("PRG chan %2u num %2u", channel, program);
}

void MidiMessageLogger::onChannelPressureChange(uint8_t channel, uint8_t value)
{
    LOG_INFO_PARAMS("CHP chan %2u val %2u", channel, value);
}

void MidiMessageLogger::onPitchBendChange(uint8_t channel, uint16_t value)
{
    LOG_INFO_PARAMS(" PB chan %2u val %5u", channel, value);
}
