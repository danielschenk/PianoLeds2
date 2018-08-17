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
 */

#include "Scheduler.h"

Scheduler::Scheduler()
    : m_queue()
    , m_mutex()
{
}

Scheduler::~Scheduler()
{
}

void Scheduler::schedule(Scheduler::TTask task)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_queue.push(task);
}

bool Scheduler::executeOne()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(!m_queue.empty())
    {
        // Call the task at the front of the queue
        m_queue.front()();
        // Remove it
        m_queue.pop();
        return true;
    }
    else
    {
        return false;
    }
}

bool Scheduler::executeAll()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    bool executed = false;
    while(!m_queue.empty())
    {
        // Call the task at the front of the queue
        m_queue.front()();
        // Remove it
        m_queue.pop();
        executed = true;
    }

    return executed;
}
