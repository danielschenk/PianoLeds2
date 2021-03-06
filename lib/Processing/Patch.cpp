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
 */

#include <Json11Helper.h>

#include "Patch.h"
#include "IProcessingBlockFactory.h"

Patch::Patch(const IProcessingBlockFactory& processingBlockFactory)
    : IPatch()
    , m_mutex()
    , m_hasBankAndProgram(false)
    , m_bank(0)
    , m_program(0)
    , m_name("Untitled Patch")
    , m_processingChain(processingBlockFactory.createProcessingChain())
    , m_processingBlockFactory(processingBlockFactory)
{
}

Patch::~Patch()
{
    delete m_processingChain;
}

IProcessingChain& Patch::getProcessingChain() const
{
    return *m_processingChain;
}

Json Patch::convertToJson() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    Json::object converted;
    converted[IJsonConvertible::c_objectTypeKey] = getObjectType();

    // Add items specific for Patch
    converted[c_hasBankAndProgramJsonKey] = m_hasBankAndProgram;
    converted[c_bankJsonKey] = m_bank;
    converted[c_programJsonKey] = m_program;
    converted[c_nameJsonKey] = m_name;

    // Add processing chain
    converted[c_processingChainJsonKey] = m_processingChain->convertToJson();

    return converted;
}

void Patch::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Get items specific for Patch
    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(c_hasBankAndProgramJsonKey, m_hasBankAndProgram);
    helper.getItemIfPresent(c_programJsonKey, m_program);
    helper.getItemIfPresent(c_bankJsonKey, m_bank);
    helper.getItemIfPresent(c_nameJsonKey, m_name);
    
    // Get processing chain
    Json::object convertedProcessingChain;
    if(helper.getItemIfPresent(c_processingChainJsonKey, convertedProcessingChain))
    {
        m_processingChain->convertFromJson(convertedProcessingChain);
    }
    else
    {
        // Reset to default.
        delete m_processingChain;
        m_processingChain = m_processingBlockFactory.createProcessingChain();
    }
}

std::string Patch::getObjectType() const
{
    return c_typeName;
}

void Patch::activate()
{
    m_processingChain->activate();
}

void Patch::deactivate()
{
    m_processingChain->deactivate();
}

void Patch::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    m_processingChain->execute(strip, noteToLightMap);
}

uint8_t Patch::getBank() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_bank;
}

void Patch::setBank(uint8_t bank)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_bank = bank;
}

bool Patch::hasBankAndProgram() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_hasBankAndProgram;
}

uint8_t Patch::getProgram() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_program;
}

void Patch::clearBankAndProgram()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_hasBankAndProgram = false;
}

void Patch::setProgram(uint8_t program)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_program = program;
    m_hasBankAndProgram = true;
}

std::string Patch::getName() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_name;
}

void Patch::setName(const std::string name)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_name = name;
}
