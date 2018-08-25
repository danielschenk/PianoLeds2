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

#include "BaseTask.h"

BaseTask::BaseTask()
    : m_taskHandle(NULL)
    , m_terminate(0)
{
}

BaseTask::~BaseTask()
{
    assert(m_taskHandle == NULL);
}

void BaseTask::start(const char* name,
                     uint32_t stackSize,
                     UBaseType_t priority)
{
    assert(m_taskHandle == NULL);

    xTaskCreate(&BaseTask::taskFunction,
                name,
                stackSize,
                this,
                priority,
                &m_taskHandle);

    assert(m_taskHandle != NULL);
}

void BaseTask::terminate()
{
    m_terminate = 1;
}

TaskHandle_t BaseTask::getTaskHandle() const
{
    return m_taskHandle;
}

void BaseTask::taskFunction(void* pvParameters)
{
    // pvPameters points to the instance
    BaseTask* instance(static_cast<BaseTask*>(pvParameters));

    while(instance->m_terminate == 0)
    {
        instance->run();
    }

    vTaskDelete(instance->m_taskHandle);
    instance->m_taskHandle = NULL;
}
