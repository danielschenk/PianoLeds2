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

#ifndef ESP32APPLICATION_MIDITASK_H_
#define ESP32APPLICATION_MIDITASK_H_

#include "BaseTask.h"

class ArduinoMidiInput;

/**
 * FreeRTOS task processing incoming MIDI bytes.
 */
class MidiTask
    : public BaseTask
{
public:
    /**
     * Constructor.
     *
     * @param midiInput     The midi input to use
     * @param stackSize     Stack size in words
     * @param priority      Priority
     */
    MidiTask(ArduinoMidiInput& midiInput,
             uint32_t stackSize,
             UBaseType_t priority);

    /**
     * Destructor.
     */
    virtual ~MidiTask() = default;

    // Prevent implicit default constructors and assignment operator.
    MidiTask() = delete;
    MidiTask(const MidiTask&) = delete;
    MidiTask& operator=(const MidiTask&) = delete;

    /**
     * Wake up the thread.
     */
    void wake();

private:
    virtual void run();

    ArduinoMidiInput& m_midiInput;
};

#endif /* ESP32APPLICATION_MIDITASK_H_ */
