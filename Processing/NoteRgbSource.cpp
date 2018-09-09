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
 */

#include <functional>

#include <Common/Logging.h>
#include <Common/Utilities/Json11Helper.h>

#include "Interfaces/IRgbFunctionFactory.h"
#include "NoteRgbSource.h"
#include "LinearRgbFunction.h"

#define LOGGING_COMPONENT "NoteRgbSource"

NoteRgbSource::NoteRgbSource(IMidiInput& midiInput, const IRgbFunctionFactory& rgbFunctionFactory)
    : m_mutex()
    , m_active()
    , m_usingPedal(true)
    , m_rgbFunctionFactory(rgbFunctionFactory)
    , m_midiInput(midiInput)
    , m_channel(0)
    , m_scheduler()
    , m_noteState()
    , m_pedalPressed(false)
    , m_rgbFunction(new LinearRgbFunction({255, 0}, {255, 0}, {255, 0}))
{
    m_midiInput.subscribe(*this);
}

NoteRgbSource::~NoteRgbSource()
{
    m_midiInput.unsubscribe(*this);
    delete m_rgbFunction;
}

void NoteRgbSource::activate()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_active = true;
}

void NoteRgbSource::deactivate()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Make sure no notes stay active. Handle remaining events first.
    m_scheduler.executeAll();
    for(auto& noteState : m_noteState)
    {
        noteState.pressed = false;
        noteState.sounding = false;
    }

    m_active = false;
}

void NoteRgbSource::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    m_scheduler.executeAll();

    for(auto pair : noteToLightMap)
    {
        // first: note number, second: light number
        if(m_rgbFunction != nullptr && pair.second < strip.size())
        {
            strip[pair.second] = m_rgbFunction->calculate(m_noteState[pair.first], 0 /* TODO pass actual time */);
        }
    }
}

void NoteRgbSource::onNoteChange(uint8_t channel, uint8_t number, uint8_t velocity, bool on)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(!m_active)
    {
        return;
    }

    auto fn = [=]() {
        if(channel == m_channel)
        {
            if(on)
            {
                m_noteState[number].pressDownVelocity = velocity;
                m_noteState[number].pressed = true;
                m_noteState[number].sounding = true;
            }
            else
            {
                m_noteState[number].pressed = false;
                if(!m_pedalPressed)
                {
                    m_noteState[number].sounding = false;
                }
            }
        }
    };
    m_scheduler.schedule(fn);
}

void NoteRgbSource::onControlChange(uint8_t channel, IMidiInput::TControllerNumber number, uint8_t value)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(!m_active)
    {
        return;
    }

    // Don't cause scheduling overhead when it's an unimportant controller number.
    // Channel check must be scheduled as it uses a member
    if(number == IMidiInterface::DAMPER_PEDAL)
    {
        auto fn = [=]() {
            if(channel == m_channel && m_usingPedal)
            {
                m_pedalPressed = (value >= 64);
                if(!m_pedalPressed)
                {
                    // Stop all notes which are sounding due to pedal only
                    for(int note = 0; note < IMidiInterface::c_numNotes; ++note)
                    {
                        if(!m_noteState[note].pressed)
                        {
                            m_noteState[note].sounding = false;
                        }
                    }
                }
            }
        };
        m_scheduler.schedule(fn);
    }
}

void NoteRgbSource::onProgramChange(uint8_t channel, uint8_t program)
{
    // ignore
}

void NoteRgbSource::onChannelPressureChange(uint8_t channel, uint8_t value)
{
    // ignore
}

void NoteRgbSource::onPitchBendChange(uint8_t channel, uint16_t value)
{
    // ignore
}

uint8_t NoteRgbSource::getChannel() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_channel;
}

void NoteRgbSource::setChannel(uint8_t channel)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_channel = channel;
}

bool NoteRgbSource::isUsingPedal() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_usingPedal;
}

void NoteRgbSource::setUsingPedal(bool usingPedal)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_usingPedal = usingPedal;
}

void NoteRgbSource::setRgbFunction(IRgbFunction* rgbFunction)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // We have ownership
    delete m_rgbFunction;
    m_rgbFunction = rgbFunction;
}

Json NoteRgbSource::convertToJson() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    Json::object json;
    json[IJsonConvertible::c_objectTypeKey] = getObjectType();
    json[c_usingPedalJsonKey] = m_usingPedal;
    json[c_channelJsonKey] = m_channel;
    if(m_rgbFunction != nullptr)
    {
        json[c_rgbFunctionJsonKey] = m_rgbFunction->convertToJson();
    }

    return json;
}

void NoteRgbSource::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(c_usingPedalJsonKey, m_usingPedal);
    helper.getItemIfPresent(c_channelJsonKey, m_channel);

    Json::object convertedRgbFunction;
    if(helper.getItemIfPresent(c_rgbFunctionJsonKey, convertedRgbFunction))
    {
        delete m_rgbFunction;
        m_rgbFunction = m_rgbFunctionFactory.createRgbFunction(convertedRgbFunction);
    }
}

std::string NoteRgbSource::getObjectType() const
{
    return IProcessingBlock::c_typeNameNoteRgbSource;
}
