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
