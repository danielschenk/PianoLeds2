/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 * @brief Interface for objects which can be converted to/from JSON.
 */

#ifndef PROCESSING_INTERFACES_IJSONCONVERTIBLE_H_
#define PROCESSING_INTERFACES_IJSONCONVERTIBLE_H_

#include <string>
#include <json11.hpp>
// for convenience
using Json = json11::Json;

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
    virtual Json convertToJson() const = 0;

    /**
     * Convert object from JSON.
     *
     * @param[in]   rConverted  The JSON object containing the persistent properties of the object.
     */
    virtual void convertFromJson(const Json& rConverted) = 0;

protected:
    /**
     * Get the object type.
     *
     * @return The object type.
     */
    virtual std::string getObjectType() const = 0;
};


#endif /* PROCESSING_INTERFACES_IJSONCONVERTIBLE_H_ */
