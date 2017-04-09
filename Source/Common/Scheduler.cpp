/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
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
