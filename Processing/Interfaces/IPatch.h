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
 * @brief Interface to a patch.
 */

#include "IProcessingBlock.h"
#include "ProcessingTypes.h"

class IPatch
    : public virtual IProcessingBlock
{
public:
    /**
     * Destructor.
     */
    virtual ~IPatch()
    {
    }

    /**
     * Check if the patch has a valid bank and program number.
     * 
     * @return  True if the patch has a valid bank and program number.
     */
    virtual bool hasBankAndProgram() const = 0;

    /**
     * Get the bank number.
     */
    virtual uint8_t getBank() const = 0;

    /**
     * Set the bank number.
     */
    virtual void setBank(uint8_t bank) = 0;

    /**
     * Get the program number.
     */
    virtual uint8_t getProgram() const = 0;

    /**
     * Set the program number.
     * 
     * @post Will mark the bank and program number valid.
     */
    virtual void setProgram(uint8_t program) = 0;

    /**
     * Clear the bank and program number.
     */
    virtual void clearBankAndProgram() = 0;
    
    /**
     * Get the name.
     */
    virtual std::string getName() const = 0;

    /**
     * Set the name.
     */
    virtual void setName(std::string name) = 0;
};
