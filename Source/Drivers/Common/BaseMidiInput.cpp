/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 */

#include "BaseMidiInput.h"

BaseMidiInput::BaseMidiInput()
    : m_nextOnOffToken(0)
    , m_noteOnOffSubscribers()
    , m_nextControlChangeToken(0)
    , m_controlChangeSubscribers()
{
}

BaseMidiInput::~BaseMidiInput()
{
}

IMidiInput::TSubscriptionToken BaseMidiInput::subscribeNoteOnOff(IMidiInput::TNoteOnOffFunction callback)
{
    // If the counter wrapped around, and there are very old subscriptions in the list (rare),
    // we might be adding an existing token. Try to find a new one in that case.
    while(m_noteOnOffSubscribers.find(m_nextOnOffToken) != m_noteOnOffSubscribers.cend())
    {
        m_nextOnOffToken++;
    }
    TSubscriptionToken newToken = m_nextOnOffToken;
    m_noteOnOffSubscribers[newToken] = callback;

    m_nextOnOffToken++;

    return newToken;
}

void BaseMidiInput::unsubscribeNoteOnOff(IMidiInput::TSubscriptionToken token)
{
    m_noteOnOffSubscribers.erase(token);
}

IMidiInput::TSubscriptionToken BaseMidiInput::subscribeControlChange(IMidiInput::TControlChangeFunction callback)
{
    // If the counter wrapped around, and there are very old subscriptions in the list (rare),
    // we might be adding an existing token. Try to find a new one in that case.
    while(m_noteOnOffSubscribers.find(m_nextOnOffToken) != m_noteOnOffSubscribers.cend())
    {
        m_nextControlChangeToken++;
    }
    TSubscriptionToken newToken = m_nextControlChangeToken;
    m_controlChangeSubscribers[newToken] = callback;

    m_nextControlChangeToken++;

    return newToken;
}

void BaseMidiInput::unsubscribeControlChange(IMidiInput::TSubscriptionToken token)
{
    m_controlChangeSubscribers.erase(token);
}

void BaseMidiInput::notifyNoteOnOff(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const
{
    for(const auto& it : m_noteOnOffSubscribers)
    {
        it.second(channel, pitch, velocity, on);
    }
}

void BaseMidiInput::notifyControlChange(uint8_t channel, IMidiInterface::TControllerNumber controller, uint8_t value) const
{
    for(const auto& it : m_controlChangeSubscribers)
    {
        it.second(channel, controller, value);
    }
}
