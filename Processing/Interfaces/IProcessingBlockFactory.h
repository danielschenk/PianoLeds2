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
 * @brief Interface for processing block factories.
 */

#ifndef PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_
#define PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_

#include <json11.hpp>
// for convenience
using Json = json11::Json;

class IProcessingBlock;
class IPatch;
class IProcessingChain;

/**
 * Interface for processing block factories.
 */
class IProcessingBlockFactory
{
public:
    /**
     * Destructor.
     */
    virtual ~IProcessingBlockFactory()
    {
    }

    /**
     * Create a processing block from the given JSON.
     *
     * @param[in]   rConverted  JSON object containing the persistent properties.
     */
    virtual IProcessingBlock* createProcessingBlock(const Json& rConverted) const = 0;

    /**
     * Create a new patch.
     */
    virtual IPatch* createPatch() const = 0;

    /**
     * Create a patch from the given JSON.
     *
     * @param[in]   rConverted  JSON object containing the persistent properties.
     */
    virtual IPatch* createPatch(const Json& rConverted) const = 0;

    /**
     * Create a new processing chain.
     */
    virtual IProcessingChain* createProcessingChain() const = 0;
};


#endif /* PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_ */
