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
