/**
 * @file
 *
 * MIT License
 *
 * @copyright (c) 2019 Daniel Schenk <danielschenk@users.noreply.github.com>
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

#include "Model.h"

#include <utility>

Model::UpdateObserverList::TSubscriptionToken Model::subscribe(Model::TUpdateCallback callback)
{
    std::lock_guard<std::mutex> lock(m_observersMutex);

    return m_observers.subscribe(std::move(callback));
}

void Model::unsubscribe(UpdateObserverList::TSubscriptionToken token)
{
    std::lock_guard<std::mutex> lock(m_observersMutex);

    m_observers.unsubscribe(token);
}

void Model::notifyObservers() const
{
    std::lock_guard<std::mutex> lock(m_observersMutex);

    m_observers.notifyObservers();
}
