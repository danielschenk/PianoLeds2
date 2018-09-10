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
                 uint16_t dataPin,
                 uint16_t clockPin,
                 uint32_t stackSize,
                 UBaseType_t priority)
    : BaseTask()
    , m_strip(stripSize, dataPin, clockPin)
{
    start("led", stackSize, priority);
}

LedTask::~LedTask()
{
}

void LedTask::onStripUpdate(const Processing::TRgbStrip& strip)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        unsigned int numLedsToSet(std::min(strip.size(), static_cast<unsigned int>(m_strip.numPixels())));
        for(unsigned int ledNumber(0); ledNumber < numLedsToSet; ++ledNumber)
        {
            auto color(strip[ledNumber]);
            m_strip.setPixelColor(ledNumber, color.r, color.g, color.b);
        }
    }

    xTaskNotifyGive(getTaskHandle());
}

void LedTask::run()
{
    // Wait until a strip update comes, or the auto-refresh interval has been reached.
    ulTaskNotifyTake(pdTRUE, c_autoRefreshInterval);

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strip.show();
    }
}
