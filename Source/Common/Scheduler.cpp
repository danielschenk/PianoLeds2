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
