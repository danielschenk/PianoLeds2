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

#ifndef ESP32APPLICATION_LEDTASK_H_
#define ESP32APPLICATION_LEDTASK_H_

#include <mutex>
#include <Adafruit_WS2801.h>

#include "BaseTask.h"
#include "Processing/Concert.h"

/**
 * Task which performs the output to the LED strip.
 */
class LedTask
    : public BaseTask
    , public Concert::IObserver
{
public:
    /**
     * Constructor.
     *
     * @param concert   Concert instance to subscribe for LED updates and check strip size
     * @param dataPin   Pin number where the data input of the strip is connected, -1 for hardware SPI
     * @param clockPin  Pin number where the clock input of the strip is connected, -1 for hardware SPI
     * @param stackSize Stack size in words
     * @param priority  Priority
     */
    LedTask(Concert& concert,
            int16_t dataPin,
            int16_t clockPin,
            uint32_t stackSize,
            UBaseType_t priority);

    /**
     * Destructor.
     */
    virtual ~LedTask();

    // Concert::IObserver implementation
    virtual void onStripUpdate(const Processing::TRgbStrip& strip);

private:
    // BaseTask implementation
    virtual void run();

    static constexpr TickType_t c_autoRefreshInterval = 100;

    /** Values stored upon last update. */
    Processing::TRgbStrip m_pendingValues;

    /** The LED strip driver. */
    Adafruit_WS2801 m_strip;

    /** Mutex protecting the pending values. */
    mutable std::mutex m_mutex;

    Concert& m_concert;
};

#endif /* ESP32APPLICATION_LEDTASK_H_ */
