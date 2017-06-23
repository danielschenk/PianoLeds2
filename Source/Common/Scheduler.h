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
 * 
 * @brief Scheduler.
 */

#ifndef COMMON_SCHEDULER_H_
#define COMMON_SCHEDULER_H_

#include <functional>
#include <queue>
#include <mutex>

/**
 * Scheduler.
 */
class Scheduler
{
public:
    /** Type for scheduled tasks. */
    typedef std::function<void()> TTask;

    /**
     * Constructor.
     */
    Scheduler();

    /**
     * Destructor.
     */
    virtual ~Scheduler();

    /**
     * Schedule a task.
     *
     * @param   [in]    task    The task to schedule.
     */
    void schedule(TTask task);

    /**
     * Execute one task from the queue.
     *
     * @retval  true    A task was executed.
     * @retval  false   The queue was empty.
     */
    bool executeOne();

    /**
     * Execute all tasks from the queue.
     *
     * @retval  true    One or more tasks were executed.
     * @retval  false   The queue was empty.
     */
    bool executeAll();

private:
    /** The queue of tasks. */
    std::queue<TTask> m_queue;

    /** The mutex to protect the queue. */
    std::mutex m_mutex;
};

#endif /* COMMON_SCHEDULER_H_ */
