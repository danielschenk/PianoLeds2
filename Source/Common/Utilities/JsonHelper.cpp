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
 */

#include <map>
#include <cstdint>
#include <string>

#include "JsonHelper.h"

JsonHelper::JsonHelper(std::string user, nlohmann::json& rJson)
    : m_user(user)
    , m_rJson(rJson)
{
}

JsonHelper::JsonHelper(const char* user, nlohmann::json& rJson)
    : m_user(user)
    , m_rJson(rJson)
{
}

JsonHelper::~JsonHelper()
{
}
