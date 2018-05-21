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

#include <Common/Logging.h>

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
     * @param[in]   rJson           JSON object to work with.
     * @param[in]   logMissingKeys  If missing keys should be logged.
     */
    Json11Helper(std::string user, const Json& rJson, bool logMissingKeys = true);
    Json11Helper(const char* user, const Json& rJson, bool logMissingKeys = true);

    virtual ~Json11Helper();

    template<typename T>
    bool getItemIfPresent(std::string key, T& rTarget) const
    {
        if(!m_rJson[key].is_null())
        {
            return getItem(key, rTarget);
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
    bool getInt(std::string key, IntegerType& rTarget) const
    {
        const auto& rItem = m_rJson[key];

        if(rItem.is_number())
        {
            rTarget = static_cast<IntegerType>(rItem.int_value());
            return true;
        }
        else
        {
            LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a number", m_user.c_str(), key.c_str());
            return false;
        }
    }

    template <typename FloatType>
    bool getFloat(std::string key, FloatType& rTarget) const
    {
        const auto& rItem = m_rJson[key];

        if(rItem.is_number())
        {
            rTarget = static_cast<FloatType>(rItem.number_value());
            return true;
        }
        else
        {
            LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a number", m_user.c_str(), key.c_str());
            return false;
        }
    }

    bool getItem(std::string key, int& rTarget) const;
    bool getItem(std::string key, uint8_t& rTarget) const;
    bool getItem(std::string key, uint16_t& rTarget) const;
    bool getItem(std::string key, float& rTarget) const;
    bool getItem(std::string key, double& rTarget) const;
    bool getItem(std::string key, bool& rTarget) const;
    bool getItem(std::string key, std::string& rTarget) const;
    bool getItem(std::string key, Json::object& rTarget) const;
    bool getItem(std::string key, Json::array& rTarget) const;

    std::string     m_user;
    const Json&     m_rJson;
    bool            m_logMissingKeys;
};

#undef LOGGING_COMPONENT

#endif /* COMMON_UTILITIES_JSON11HELPER_H_ */
