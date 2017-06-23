/**
 * @file
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 * @brief Interface for objects which can be converted to/from JSON.
 */

#ifndef PROCESSING_INTERFACES_IJSONCONVERTIBLE_H_
#define PROCESSING_INTERFACES_IJSONCONVERTIBLE_H_

#include <string>
#include <json.hpp>

// for convenience
using json = nlohmann::json;

/**
 * Interface for objects which can be converted to/from JSON.
 */
class IJsonConvertible
{
public:
    static constexpr const char* c_objectTypeKey = "objectType";

    /**
     * Destructor.
     */
    virtual ~IJsonConvertible()
    {
    }

    /**
     * Convert object to JSON.
     *
     * @return The JSON object containing the persistent properties of the object.
     */
    virtual json convertToJson() const = 0;

    /**
     * Convert object from JSON.
     *
     * @param[in]   json    The JSON object containing the persistent properties of the object.
     */
    virtual void convertFromJson(json json) = 0;
};


#endif /* PROCESSING_INTERFACES_IJSONCONVERTIBLE_H_ */
