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

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "MidiTask.h"
#include "ArduinoMidiInput.h"

MidiTask::MidiTask(ArduinoMidiInput& midiInput,
                   uint32_t stackSize,
                   UBaseType_t priority)
    : BaseTask()
    , m_midiInput(midiInput)
{
    start("midi", stackSize, priority);
}

void MidiTask::wake()
{
    xTaskNotifyGive(getTaskHandle());
}

void MidiTask::run()
{
    // TODO workaround for lack of serialEvent implementation on ESP32.
    // Wait for event for a maximum of 100 ms.
    // This effectively is a polling implementation instead of an event-driven one.
    ulTaskNotifyTake(pdTRUE, 100);

    // Process any data.
    m_midiInput.run();
}
