/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
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
