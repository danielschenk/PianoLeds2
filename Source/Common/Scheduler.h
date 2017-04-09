/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
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
