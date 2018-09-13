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

#include <string>
#include <cstdio>

#include "Common/Logging.h"
#include "StripChangeLogger.h"

#define LOGGING_COMPONENT "StripChangeLogger"

StripChangeLogger::StripChangeLogger(Concert& concert)
    : m_concert(concert)
    , m_previous()
{
    m_concert.subscribe(*this);
}

StripChangeLogger::~StripChangeLogger()
{
    m_concert.unsubscribe(*this);
}

void StripChangeLogger::onStripUpdate(const Processing::TRgbStrip& strip)
{
    bool log(false);

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if(strip != m_previous)
        {
            m_previous = strip;
            log = true;
        }
    }

    if(log)
    {
        std::string msg("Strip update:\r\n");
        uint16_t ledNumber(0);
        for(auto led : strip)
        {
            // Fits: nnn: rrr ggg bbb[CR][LF][NUL]
            char buf[19];
            snprintf(buf, sizeof(buf), "%3u: %3u %3u %3u\r\n",
                     ledNumber, led.r, led.g, led.b);
            msg.append(buf);
            ++ledNumber;
        }

        LOG_DEBUG_PARAMS("%s", msg.c_str());
    }
}
