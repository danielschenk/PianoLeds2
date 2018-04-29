/**
 * @file
 * @copyright (c) Daniel Schenk, 2018
 * This file is part of MLC2.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
