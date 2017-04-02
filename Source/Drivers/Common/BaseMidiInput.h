/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Base class for MIDI inputs, implementing message subscription mechanism.
 */

#ifndef DRIVERS_COMMON_BASEMIDIINPUT_H_
#define DRIVERS_COMMON_BASEMIDIINPUT_H_

#include <map>

#include "../Interfaces/IMidiInput.h"

/**
 * Base class for MIDI inputs, implementing message subscription mechanism.
 */
class BaseMidiInput
    : public IMidiInput
{
public:
    /**
     * Constructor.
     */
    BaseMidiInput();

    /**
     * Destructor.
     */
    virtual ~BaseMidiInput();

    // Prevent implicit copy constructor and assignment operator.
    BaseMidiInput(const BaseMidiInput&) = delete;
    BaseMidiInput& operator=(const BaseMidiInput&) = delete;

    // IMidiInput implementation.
    virtual TSubscriptionToken subscribeNoteOnOff(TNoteOnOffFunction callback);
    virtual void unsubscribeNoteOnOff(TSubscriptionToken token);
    virtual TSubscriptionToken subscribeControlChange(TControlChangeFunction callback);
    virtual void unsubscribeControlChange(TSubscriptionToken token);

protected:
    /**
     * Notify all note on/off subscribers.
     *
     * @param   channel     [in]    The channel the message was received on.
     * @param   pitch       [in]    The pitch of the note.
     * @param   pitch       [in]    The pitch of the note.
     */
    virtual void notifyNoteOnOff(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const;

    /**
     * Notify all control change subscribers.
     *
     * @param   channel     [in]    The channel the message was received on.
     * @param   controller  [in]    The number of the controller.
     * @param   value       [in]    The value of the controller.
     */
    virtual void notifyControlChange(uint8_t channel, IMidiInput::TControllerNumber control, uint8_t value) const;

private:
    /** The next value to be used for an on/off subscription. */
    TSubscriptionToken m_nextOnOffToken;

    /** Collection of note on/off subscribers. */
    std::map<IMidiInput::TSubscriptionToken, IMidiInput::TNoteOnOffFunction> m_noteOnOffSubscribers;

    /** The next value to be used for a control change subscription. */
    TSubscriptionToken m_nextControlChangeToken;

    /** Collection of control change subscribers. */
    std::map<IMidiInput::TSubscriptionToken, IMidiInput::TControlChangeFunction> m_controlChangeSubscribers;
};

#endif /* DRIVERS_COMMON_BASEMIDIINPUT_H_ */
