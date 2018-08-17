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

#include "Json11Helper.h"

#define LOGGING_COMPONENT "Json11Helper"

Json11Helper::Json11Helper(std::string user, const Json& rJson, bool logMissingKeys)
    : m_user(user)
    , m_rJson(rJson)
    , m_logMissingKeys(logMissingKeys)
{
}

Json11Helper::Json11Helper(const char* user, const Json& rJson, bool logMissingKeys)
    : m_user(user)
    , m_rJson(rJson)
    , m_logMissingKeys(logMissingKeys)
{
    if(!m_rJson.is_object())
    {
        LOG_ERROR_PARAMS("%s: Passed object not a JSON object", m_user.c_str());
    }
}

Json11Helper::~Json11Helper()
{
}

bool Json11Helper::getItem(std::string key, int& rTarget) const
{
    return getInt(key, rTarget);
}

bool Json11Helper::getItem(std::string key, uint8_t& rTarget) const
{
    return getInt(key, rTarget);
}

bool Json11Helper::getItem(std::string key, uint16_t& rTarget) const
{
    return getInt(key, rTarget);
}

bool Json11Helper::getItem(std::string key, float& rTarget) const
{
    return getFloat(key, rTarget);
}

bool Json11Helper::getItem(std::string key, double& rTarget) const
{
    return getFloat(key, rTarget);
}

bool Json11Helper::getItem(std::string key, bool& rTarget) const
{
    const auto& rItem = m_rJson[key];

    if(rItem.is_bool())
    {
        rTarget = rItem.bool_value();
        return true;
    }
    else
    {
        LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a boolean", m_user.c_str(), key.c_str());
        return false;
    }
}

bool Json11Helper::getItem(std::string key, std::string& rTarget) const
{
    const auto& rItem = m_rJson[key];

    if(rItem.is_string())
    {
        rTarget = rItem.string_value();
        return true;
    }
    else
    {
        LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a string", m_user.c_str(), key.c_str());
        return false;
    }
}

bool Json11Helper::getItem(std::string key, Json::object& rTarget) const
{
    const auto& rItem = m_rJson[key];

    if(rItem.is_object())
    {
        rTarget = rItem.object_items();
        return true;
    }
    else
    {
        LOG_ERROR_PARAMS("%s: JSON value with key '%s' not an object", m_user.c_str(), key.c_str());
        return false;
    }
}

bool Json11Helper::getItem(std::string key, Json::array& rTarget) const
{
    const auto& rItem = m_rJson[key];

    if(rItem.is_array())
    {
        rTarget = rItem.array_items();
        return true;
    }
    else
    {
        LOG_ERROR_PARAMS("%s: JSON value with key '%s' not an array", m_user.c_str(), key.c_str());
        return false;
    }
}
