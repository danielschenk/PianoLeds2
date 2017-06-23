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

#include "Interfaces/IRgbFunctionFactory.h"
#include "NoteRgbSource.h"
#include "LinearRgbFunction.h"

#define LOGGING_COMPONENT "NoteRgbSource"

NoteRgbSource::NoteRgbSource(IMidiInput& rMidiInput, const Processing::TNoteToLightMap& rNoteToLightMap, const IRgbFunctionFactory& rRgbFunctionFactory)
    : m_usingPedal(true)
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
    return m_channel;
}

void NoteRgbSource::setChannel(uint8_t channel)
{
    m_channel = channel;
}

bool NoteRgbSource::isUsingPedal() const
{
    return m_usingPedal;
}

void NoteRgbSource::setUsingPedal(bool usingPedal)
{
    m_usingPedal = usingPedal;
}

void NoteRgbSource::setRgbFunction(IRgbFunction* pRgbFunction)
{
    // We have ownership
    delete m_pRgbFunction;
    m_pRgbFunction = pRgbFunction;
}

json NoteRgbSource::convertToJson() const
{
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
    if(converted.count(c_usingPedalJsonKey) > 0)
    {
        m_usingPedal = converted[c_usingPedalJsonKey];
    }
    else
    {
        LOG_ERROR("convertFromJson: Missing usingPedal property");
    }
    if(converted.count(c_channelJsonKey) > 0)
    {
        m_channel = converted[c_channelJsonKey];
    }
    else
    {
        LOG_ERROR("convertFromJson: Missing channel property");
    }
    if(converted.count(c_rgbFunctionJsonKey) > 0)
    {
        delete m_pRgbFunction;
        m_pRgbFunction = m_rRgbFunctionFactory.createRgbFunction(converted[c_rgbFunctionJsonKey]);
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
           (it.key() != c_rgbFunctionJsonKey))
        {
            LOG_WARNING_PARAMS("convertFromJson: unknown property '%s'", it.key().c_str());
        }
    }
}
