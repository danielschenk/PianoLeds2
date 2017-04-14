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
#include <Common/ObserverList.h>

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
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   pitch       The pitch of the note.
     * @param[in]   velocity    The pitch of the note.
     */
    virtual void notifyNoteOnOff(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const;

    /**
     * Notify all control change subscribers.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   controller  The number of the controller.
     * @param[in]   value       The value of the controller.
     */
    virtual void notifyControlChange(uint8_t channel, IMidiInput::TControllerNumber control, uint8_t value) const;

private:
    /** Collection of note on/off subscribers. */
    ObserverList<uint8_t, uint8_t, uint8_t, bool> m_noteOnOffSubscribers;

    /** Collection of control change subscribers. */
    ObserverList<uint8_t, IMidiInput::TControllerNumber, uint8_t> m_controlChangeSubscribers;
};

#endif /* DRIVERS_COMMON_BASEMIDIINPUT_H_ */
