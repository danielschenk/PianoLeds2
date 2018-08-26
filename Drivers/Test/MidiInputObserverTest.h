/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2018 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 */

#ifndef MIDIINPUTOBSERVERTEST_H_
#define MIDIINPUTOBSERVERTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../Mock/MockMidiInput.h"

using ::testing::_;
using ::testing::NiceMock;

ACTION_P(StoreArg0Address, target) { *target = &arg0; }

/**
 * Base fixture for tests of classes which implement @ref IMidiInput::IObserver
 */
class MidiInputObserverTest
    : public ::testing::Test
{
public:
    MidiInputObserverTest()
        : m_mockMidiInput()
        , m_observer(nullptr)
    {
        // Capture observer so we can simulate events
        ON_CALL(m_mockMidiInput, subscribe(_))
            .WillByDefault(StoreArg0Address(&m_observer));
    }

    virtual ~MidiInputObserverTest() = default;

    NiceMock<MockMidiInput> m_mockMidiInput;
    IMidiInput::IObserver* m_observer;
};


#endif /* MIDIINPUTOBSERVERTEST_H_ */
