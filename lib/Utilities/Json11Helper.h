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
 *
 * @brief Helper class to fetch JSON items with type checking.
 */

#ifndef COMMON_UTILITIES_JSON11HELPER_H_
#define COMMON_UTILITIES_JSON11HELPER_H_

#include <string>
#include <json11.hpp>
// for convenience
using Json = json11::Json;

#include "Logging.h"

#define LOGGING_COMPONENT "Json11Helper"

/**
 * @brief Helper class to fetch JSON items with type checking.
 */
class Json11Helper
{
public:
    /**
     * Constructor.
     *
     * @param[in]   user            Name of the user for logging errors.
     * @param[in]   json            JSON object to work with.
     * @param[in]   logMissingKeys  If missing keys should be logged.
     */
    Json11Helper(std::string user, const Json& json, bool logMissingKeys = true);
    Json11Helper(const char* user, const Json& json, bool logMissingKeys = true);

    virtual ~Json11Helper();

    template<typename T>
    bool getItemIfPresent(std::string key, T& target) const
    {
        if(!m_json[key].is_null())
        {
            return getItem(key, target);
        }
        else
        {
            if(m_logMissingKeys)
            {
                LOG_ERROR_PARAMS("%s: Missing JSON key '%s'", m_user.c_str(), key.c_str());
            }
            return false;
        }
    }

    Json11Helper() = delete;
    Json11Helper(const Json11Helper&) = delete;
    Json11Helper& operator=(const Json11Helper&) = delete;

private:
    template <typename IntegerType>
    bool getInt(std::string key, IntegerType& target) const
    {
        const auto& item = m_json[key];

        if(item.is_number())
        {
            target = static_cast<IntegerType>(item.int_value());
            return true;
        }
        else
        {
            LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a number", m_user.c_str(), key.c_str());
            return false;
        }
    }

    template <typename FloatType>
    bool getFloat(std::string key, FloatType& target) const
    {
        const auto& item = m_json[key];

        if(item.is_number())
        {
            target = static_cast<FloatType>(item.number_value());
            return true;
        }
        else
        {
            LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a number", m_user.c_str(), key.c_str());
            return false;
        }
    }

    bool getItem(std::string key, int& target) const;
    bool getItem(std::string key, uint8_t& target) const;
    bool getItem(std::string key, uint16_t& target) const;
    bool getItem(std::string key, float& target) const;
    bool getItem(std::string key, double& target) const;
    bool getItem(std::string key, bool& target) const;
    bool getItem(std::string key, std::string& target) const;
    bool getItem(std::string key, Json::object& target) const;
    bool getItem(std::string key, Json::array& target) const;

    std::string     m_user;
    const Json&     m_json;
    bool            m_logMissingKeys;
};

#undef LOGGING_COMPONENT

#endif /* COMMON_UTILITIES_JSON11HELPER_H_ */
