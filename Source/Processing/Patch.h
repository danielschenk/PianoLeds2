/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
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
 * @brief Class which represents a patch.
 */

#ifndef PROCESSING_PATCH_H_
#define PROCESSING_PATCH_H_

#include <string>

#include "ProcessingChain.h"

class IProcessingBlockFactory;

/**
 * Class which represents a patch.
 *
 * This class does not represent a software patch (lines of code to apply to some other code).
 * Instead, it's more like an analog synthesizer patch. It contains the configuration of the processing chain,
 * so light is generated from MIDI data in a certain way.
 */
class Patch
    : public ProcessingChain
{
public:
    /**
     * Constructor.
     *
     * @param[in]   rProcessingBlockFactory Reference to the processing block factory.
     */
    Patch(const IProcessingBlockFactory& rProcessingBlockFactory);

    /**
     * Destructor.
     */
    virtual ~Patch();

    /**
     * Copy constructor.
     */
    Patch(const Patch&);

    // Prevent implicit constructor and assignment operator.
    Patch() = delete;
    Patch& operator=(const Patch&) = delete;

    // IJsonConvertible implementation
    virtual json convertToJson() const;
    virtual void convertFromJson(json json);

    uint8_t getBank() const;
    void setBank(uint8_t bank);
    bool hasBankAndProgram() const;
    void setHasBankAndProgram(bool hasBankAndProgram);
    uint8_t getProgram() const;
    void setProgram(uint8_t program);
    std::string getName() const;
    void setName(const std::string name);

private:
    static constexpr const char* c_hasBankAndProgramJsonKey = "hasBankAndProgram";
    static constexpr const char* c_bankJsonKey              = "bank";
    static constexpr const char* c_programJsonKey           = "program";
    static constexpr const char* c_nameJsonKey              = "name";

    /** Whether this patch is bound to a bank and program number. */
    bool m_hasBankAndProgram;

    /** The bank number bound to this patch. */
    uint8_t m_bank;

    /** The program number bound to this patch. */
    uint8_t m_program;

    /** The name of the patch. */
    std::string m_name;
};

#endif /* PROCESSING_PATCH_H_ */
