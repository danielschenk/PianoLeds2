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
