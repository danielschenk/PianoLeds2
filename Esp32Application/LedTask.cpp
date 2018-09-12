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

#include <algorithm>

#include "LedTask.h"


LedTask::LedTask(uint16_t stripSize,
                 int16_t dataPin,
                 int16_t clockPin,
                 uint32_t stackSize,
                 UBaseType_t priority)
    : BaseTask()
    , m_pendingValues()
    , m_strip(stripSize)
{
    if((dataPin > -1) && (clockPin > -1))
    {
        m_strip.updatePins(static_cast<uint8_t>(dataPin), static_cast<uint8_t>(clockPin));
    }
    else
    {
        m_strip.updatePins();
    }
    m_strip.begin();

    start("led", stackSize, priority);
}

LedTask::~LedTask()
{
}

void LedTask::onStripUpdate(const Processing::TRgbStrip& strip)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        // Store the new values.
        m_pendingValues = strip;
    }

    // Notify the task that an update was received.
    xTaskNotifyGive(getTaskHandle());
}

void LedTask::run()
{
    // Wait until a strip update comes, or the auto-refresh interval has been reached.
    auto notifyGiveCount(ulTaskNotifyTake(pdTRUE, c_autoRefreshInterval));

    if (notifyGiveCount != 0)
    {
        // A strip update was received, update the colors in the driver.
        std::lock_guard<std::mutex> lock(m_mutex);

        for(unsigned int ledNumber(0); ledNumber < m_pendingValues.size(); ++ledNumber)
        {
            if(ledNumber >= m_strip.numPixels())
            {
                break;
            }

            auto color(m_pendingValues[ledNumber]);
            m_strip.setPixelColor(ledNumber, color.r, color.g, color.b);
        }
    }

    // Send latest state to strip.
    // This is done outside of the lock to prevent holding up the thread sending the
    // strip updates. The driver is accessed only in this thread anyway.
    m_strip.show();
}
