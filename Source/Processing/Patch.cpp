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
 */

#include <Common/Utilities/JsonHelper.h>

#include "Patch.h"
#include "Interfaces/IProcessingBlockFactory.h"

Patch::Patch(const IProcessingBlockFactory& rProcessingBlockFactory)
    : ProcessingChain(rProcessingBlockFactory)
    , m_hasBankAndProgram(false)
    , m_bank(0)
    , m_program(0)
    , m_name("Untitled Patch")
{
}

Patch::Patch(const Patch& rOther)
    : ProcessingChain(rOther.m_rProcessingBlockFactory)
{
    // Use JSON because the base doesn't implement a copy constructor.
    convertFromJson(rOther.convertToJson());
}

Patch::~Patch()
{
}

json Patch::convertToJson() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    // Start with contents of base
    json converted = ProcessingChain::convertToJson();
    // Overwrite object type
    converted[IProcessingBlock::c_objectTypeKey] = std::string(IProcessingBlock::c_typeNamePatch);

    // Add items specific for Patch
    converted[c_hasBankAndProgramJsonKey] = m_hasBankAndProgram;
    converted[c_bankJsonKey] = m_bank;
    converted[c_programJsonKey] = m_program;
    converted[c_nameJsonKey] = m_name;

    return converted;
}

void Patch::convertFromJson(json converted)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    // Get items specific for Patch
    JsonHelper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(c_hasBankAndProgramJsonKey, m_hasBankAndProgram);
    helper.getItemIfPresent(c_programJsonKey, m_program);
    helper.getItemIfPresent(c_bankJsonKey, m_bank);
    helper.getItemIfPresent(c_nameJsonKey, m_name);
    
    // Remove keys the base doesn't know about, to prevent warnings
    converted.erase(c_hasBankAndProgramJsonKey);
    converted.erase(c_programJsonKey);
    converted.erase(c_bankJsonKey);
    converted.erase(c_nameJsonKey);

    // Get contents of base
    ProcessingChain::convertFromJson(converted);
}

uint8_t Patch::getBank() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    return m_bank;
}

void Patch::setBank(uint8_t bank)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_bank = bank;
}

bool Patch::hasBankAndProgram() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    return m_hasBankAndProgram;
}

uint8_t Patch::getProgram() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    return m_program;
}

void Patch::clearBankAndProgram()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_hasBankAndProgram = false;
}

void Patch::setProgram(uint8_t program)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_program = program;
    m_hasBankAndProgram = true;
}

std::string Patch::getName() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    return m_name;
}

void Patch::setName(const std::string name)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_name = name;
}
