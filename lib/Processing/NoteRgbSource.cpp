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

#include "NoteRgbSource.h"

#include "IRgbFunctionFactory.h"
#include "IRgbFunction.h"
#include "ITime.h"
#include "Json11Helper.h"
#include "Logging.h"

#include <functional>

#define LOGGING_COMPONENT "NoteRgbSource"

NoteRgbSource::NoteRgbSource(IMidiInput& midiInput,
                             const IRgbFunctionFactory& rgbFunctionFactory,
                             const ITime& time)
    : rgbFunctionFactory(rgbFunctionFactory)
    , midiInput(midiInput)
    , time(time)
{
    midiInput.subscribe(*this);
}

NoteRgbSource::~NoteRgbSource()
{
    midiInput.unsubscribe(*this);
    delete rgbFunction;
}

void NoteRgbSource::activate()
{
    std::lock_guard<std::mutex> lock(mutex);

    active = true;
}

void NoteRgbSource::deactivate()
{
    std::lock_guard<std::mutex> lock(mutex);

    // Make sure no notes stay active. Handle remaining events first.
    scheduler.executeAll();
    for(auto& noteState : noteStates)
    {
        noteState.pressed = false;
        noteState.sounding = false;
    }

    active = false;
}

void NoteRgbSource::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    scheduler.executeAll();

    for(auto pair : noteToLightMap)
    {
        // first: note number, second: light number
        if(rgbFunction != nullptr && pair.second < strip.size())
        {
            strip[pair.second] += rgbFunction->calculate(noteStates[pair.first], time.getMilliseconds());
        }
    }
}

void NoteRgbSource::onNoteChange(uint8_t channel, uint8_t number, uint8_t velocity, bool on)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(!active)
    {
        return;
    }

    scheduler.schedule([=]() {
        if(channel == this->channel)
        {
            if(on)
            {
                noteStates[number].pressDownVelocity = velocity;
                noteStates[number].noteOnTimeStamp = time.getMilliseconds();
                noteStates[number].pressed = true;
                noteStates[number].sounding = true;
            }
            else
            {
                noteStates[number].pressed = false;
                if(!pedalPressed)
                {
                    noteStates[number].sounding = false;
                }
            }
        }
    });
}

void NoteRgbSource::onControlChange(uint8_t channel, IMidiInput::TControllerNumber number, uint8_t value)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(!active)
    {
        return;
    }

    // Don't cause scheduling overhead when it's an unimportant controller number.
    // Channel check must be scheduled as it uses a member
    if(number == IMidiInterface::DAMPER_PEDAL)
    {
        scheduler.schedule([=]() {
            if(channel == this->channel && usingPedal)
            {
                pedalPressed = (value >= 64);
                if(!pedalPressed)
                {
                    // Stop all notes which are sounding due to pedal only
                    for(int note = 0; note < IMidiInterface::c_numNotes; ++note)
                    {
                        if(!noteStates[note].pressed)
                        {
                            noteStates[note].sounding = false;
                        }
                    }
                }
            }
        });
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
    std::lock_guard<std::mutex> lock(mutex);

    return channel;
}

void NoteRgbSource::setChannel(uint8_t channel)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->channel = channel;
}

bool NoteRgbSource::isUsingPedal() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return usingPedal;
}

void NoteRgbSource::setUsingPedal(bool usingPedal)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->usingPedal = usingPedal;
}

void NoteRgbSource::setRgbFunction(IRgbFunction* rgbFunction)
{
    std::lock_guard<std::mutex> lock(mutex);

    // We have ownership
    delete rgbFunction;
    this->rgbFunction = rgbFunction;
}

Json NoteRgbSource::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object json;
    json[IJsonConvertible::c_objectTypeKey] = getObjectType();
    json[c_usingPedalJsonKey] = usingPedal;
    json[c_channelJsonKey] = channel;
    if(rgbFunction != nullptr)
    {
        json[c_rgbFunctionJsonKey] = rgbFunction->convertToJson();
    }

    return json;
}

void NoteRgbSource::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(c_usingPedalJsonKey, usingPedal);
    helper.getItemIfPresent(c_channelJsonKey, channel);

    Json::object convertedRgbFunction;
    if(helper.getItemIfPresent(c_rgbFunctionJsonKey, convertedRgbFunction))
    {
        delete rgbFunction;
        rgbFunction = rgbFunctionFactory.createRgbFunction(convertedRgbFunction);
    }
}

std::string NoteRgbSource::getObjectType() const
{
    return IProcessingBlock::c_typeNameNoteRgbSource;
}
