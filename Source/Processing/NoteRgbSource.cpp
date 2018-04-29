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
 *
 */

#include <functional>

#include <Common/Logging.h>
#include <Common/Utilities/JsonHelper.h>

#include "Interfaces/IRgbFunctionFactory.h"
#include "NoteRgbSource.h"
#include "LinearRgbFunction.h"

#define LOGGING_COMPONENT "NoteRgbSource"

NoteRgbSource::NoteRgbSource(IMidiInput& rMidiInput, const Processing::TNoteToLightMap& rNoteToLightMap, const IRgbFunctionFactory& rRgbFunctionFactory)
    : m_mutex()
    , m_active()
    , m_usingPedal(true)
    , m_rNoteToLightMap(rNoteToLightMap)
    , m_rRgbFunctionFactory(rRgbFunctionFactory)
    , m_rMidiInput(rMidiInput)
    , m_channel(0)
    , m_scheduler()
    , m_noteState()
    , m_pRgbFunction(new LinearRgbFunction({255, 0}, {255, 0}, {255, 0}))
{
    m_noteOnOffSubscription = m_rMidiInput.subscribeNoteOnOff(
            std::bind(&NoteRgbSource::handleNoteOnOff, this, std::placeholders::_1,  std::placeholders::_2,  std::placeholders::_3, std::placeholders::_4));
    m_controlChangeSubscription = m_rMidiInput.subscribeControlChange(
            std::bind(&NoteRgbSource::handleControlChange, this, std::placeholders::_1,  std::placeholders::_2,  std::placeholders::_3));
}

NoteRgbSource::~NoteRgbSource()
{
    m_rMidiInput.unsubscribeNoteOnOff(m_noteOnOffSubscription);
    m_rMidiInput.unsubscribeControlChange(m_controlChangeSubscription);
    delete m_pRgbFunction;
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

void NoteRgbSource::execute(Processing::TRgbStrip& strip)
{
    m_scheduler.executeAll();

    for(auto pair : m_rNoteToLightMap)
    {
        // first: note number, second: light number
        if(m_pRgbFunction != nullptr && pair.second < strip.size())
        {
            strip[pair.second] = m_pRgbFunction->calculate(m_noteState[pair.first], 0 /* TODO pass actual time */);
        }
    }
}

void NoteRgbSource::handleNoteOnOff(uint8_t channel, uint8_t number, uint8_t velocity, bool on)
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

void NoteRgbSource::handleControlChange(uint8_t channel, IMidiInput::TControllerNumber number, uint8_t value)
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

void NoteRgbSource::setRgbFunction(IRgbFunction* pRgbFunction)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // We have ownership
    delete m_pRgbFunction;
    m_pRgbFunction = pRgbFunction;
}

json NoteRgbSource::convertToJson() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    json json;
    json[IJsonConvertible::c_objectTypeKey] = std::string(IProcessingBlock::c_typeNameNoteRgbSource);
    json[c_usingPedalJsonKey] = m_usingPedal;
    json[c_channelJsonKey] = m_channel;
    if(m_pRgbFunction != nullptr)
    {
        json[c_rgbFunctionJsonKey] = m_pRgbFunction->convertToJson();
    }

    return json;
}

void NoteRgbSource::convertFromJson(json converted)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    JsonHelper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(c_usingPedalJsonKey, m_usingPedal);
    helper.getItemIfPresent(c_channelJsonKey, m_channel);

    std::string rgbFunctionJsonKey(c_rgbFunctionJsonKey);
    if(converted.count(rgbFunctionJsonKey) > 0)
    {
        delete m_pRgbFunction;
        m_pRgbFunction = m_rRgbFunctionFactory.createRgbFunction(converted[rgbFunctionJsonKey]);
    }
    else
    {
        LOG_ERROR("convertFromJson: Missing rgbFunction property");
    }

    for(auto it = converted.begin(); it != converted.end(); ++it)
    {
        if((it.key() != IJsonConvertible::c_objectTypeKey) &&
           (it.key() != c_channelJsonKey) &&
           (it.key() != c_usingPedalJsonKey) &&
           (it.key() != rgbFunctionJsonKey))
        {
            LOG_WARNING_PARAMS("convertFromJson: unknown property '%s'", it.key().c_str());
        }
    }
}
