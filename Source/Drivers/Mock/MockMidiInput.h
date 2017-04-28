/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Mock MIDI driver.
 */

#ifndef DRIVERS_MOCK_MOCKMIDIINPUT_H_
#define DRIVERS_MOCK_MOCKMIDIINPUT_H_

#include <gmock/gmock.h>

#include "../Interfaces/IMidiInput.h"

class MockMidiInput
    : public IMidiInput
{
public:
    // IMidiInterface implementation
    MOCK_CONST_METHOD0(getPortCount, unsigned int());
    MOCK_METHOD1(openPort, void(int number));

    // IMidiInput implementation
    MOCK_METHOD1(subscribeNoteOnOff, TSubscriptionToken(TNoteOnOffFunction callback));
    MOCK_METHOD1(unsubscribeNoteOnOff, void(TSubscriptionToken token));
    MOCK_METHOD1(subscribeControlChange, TSubscriptionToken(TControlChangeFunction callback));
    MOCK_METHOD1(unsubscribeControlChange, void(TSubscriptionToken token));
    MOCK_METHOD1(subscribeProgramChange, TSubscriptionToken(TProgramChangeFunction callback));
    MOCK_METHOD1(unsubscribeProgramChange, void(TSubscriptionToken token));
};

#endif /* DRIVERS_MOCK_MOCKMIDIINPUT_H_ */
