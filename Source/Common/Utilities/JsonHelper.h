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
 * @brief JSON helper.
 */

#ifndef COMMON_UTILITIES_JSONHELPER_H_
#define COMMON_UTILITIES_JSONHELPER_H_

#include <string>
#include <nlohmann/json.hpp>

#include <Common/Logging.h>

#define LOGGING_COMPONENT "JsonHelper"

class JsonHelper
{
public:
    /**
     * Constructor.
     * 
     * @param[in]   user    Name of the user for logging errors.
     * @param[in]   rJson   JSON object to work with.
     */
    JsonHelper(std::string user, nlohmann::json& rJson);
    JsonHelper(const char* user, nlohmann::json& rJson);

    virtual ~JsonHelper();

    template<typename T>
    bool getItemIfPresent(std::string key, T& rTarget) const
    {
        bool found(false);
        if(m_rJson.count(key) > 0)
        {
            found = true;
            getItem<T>(key, rTarget);
        }
        else
        {
            LOG_ERROR_PARAMS("%s: Missing JSON key '%s'", m_user.c_str(), key.c_str());
        }

        return found;
    }

    JsonHelper() = delete;
    JsonHelper(const JsonHelper&) = delete;
    JsonHelper& operator=(const JsonHelper&) = delete;

private:
    template<typename T>
    void getItem(std::string key, T& rTarget) const
    {
        rTarget = m_rJson.at(key).template get<T>();
    }

    std::string     m_user;
    nlohmann::json& m_rJson;
};

#undef LOGGING_COMPONENT

#endif // COMMON_UTILITIES_JSONHELPER_H_
