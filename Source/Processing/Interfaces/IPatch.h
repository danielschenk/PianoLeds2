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
