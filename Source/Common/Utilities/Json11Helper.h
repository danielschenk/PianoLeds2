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

#include <Common/Logging.h>

#define LOGGING_COMPONENT "Json11Helper"

class Json11Helper
{
public:
    /**
     * Constructor.
     *
     * @param[in]   user    Name of the user for logging errors.
     * @param[in]   rJson   JSON object to work with.
     */
    Json11Helper(std::string user, json11::Json& rJson);
    Json11Helper(const char* user, json11::Json& rJson);

    virtual ~Json11Helper();

    template<typename T>
    bool getItemIfPresent(std::string key, T& rTarget) const
    {
        bool found(false);
        if(!m_rJson[key].is_null())
        {
            found = true;
            return getItem(key, rTarget);
        }
        else
        {
            LOG_ERROR_PARAMS("%s: Missing JSON key '%s'", m_user.c_str(), key.c_str());
        }

        return found;
    }

    Json11Helper() = delete;
    Json11Helper(const Json11Helper&) = delete;
    Json11Helper& operator=(const Json11Helper&) = delete;

private:
    bool getItem(std::string key, int& rTarget) const;
    bool getItem(std::string key, double& rTarget) const;
    bool getItem(std::string key, bool& rTarget) const;
    bool getItem(std::string key, std::string& rTarget) const;

    std::string     m_user;
    json11::Json&   m_rJson;
};

#undef LOGGING_COMPONENT

#endif /* COMMON_UTILITIES_JSON11HELPER_H_ */
