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

#ifndef ESP32APPLICATION_BASETASK_H_
#define ESP32APPLICATION_BASETASK_H_

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/**
 * Base class for a FreeRTOS task.
 */
class BaseTask
{
public:
    // Prevent implicit constructors and assignment operator
    BaseTask(const BaseTask&) = delete;
    BaseTask& operator=(const BaseTask&) = delete;

    /**
     * Destructor.
     */
    virtual ~BaseTask();

    /**
     * Tell the task to terminate.
     */
    void terminate();

protected:
    /**
     * Constructor.
     */
    BaseTask();

    /**
     * Create and start the task.
     *
     * @param pName     Name of the task
     * @param stackSize Stack size in words
     * @param priority  Priority
     */
    void start(const char* pName,
               uint32_t stackSize,
               UBaseType_t priority);

    TaskHandle_t getTaskHandle() const;

private:
    /**
     * Run function of the task. Is repeatedly executed until termination.
     */
    virtual void run() = 0;

    /**
     * Entry point for FreeRTOS, needs to be static.
     *
     * @param pvParameters  Should point to the instance.
     */
    static void taskFunction(void* pvParameters);

    TaskHandle_t m_taskHandle;
    BaseType_t m_terminate;
};

#endif /* ESP32APPLICATION_BASETASK_H_ */
