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
    : processingChain(processingBlockFactory.createProcessingChain())
    , processingBlockFactory(processingBlockFactory)
{
}

Patch::~Patch()
{
    delete processingChain;
}

IProcessingChain& Patch::getProcessingChain() const
{
    return *processingChain;
}

Json Patch::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object converted;
    converted[IJsonConvertible::c_objectTypeKey] = getObjectType();

    // Add items specific for Patch
    converted[c_hasBankAndProgramJsonKey] = bankAndProgramSet;
    converted[c_bankJsonKey] = bank;
    converted[c_programJsonKey] = program;
    converted[c_nameJsonKey] = name;

    // Add processing chain
    converted[c_processingChainJsonKey] = processingChain->convertToJson();

    return converted;
}

void Patch::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    // Get items specific for Patch
    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(c_hasBankAndProgramJsonKey, bankAndProgramSet);
    helper.getItemIfPresent(c_programJsonKey, program);
    helper.getItemIfPresent(c_bankJsonKey, bank);
    helper.getItemIfPresent(c_nameJsonKey, name);
    
    // Get processing chain
    Json::object convertedProcessingChain;
    if(helper.getItemIfPresent(c_processingChainJsonKey, convertedProcessingChain))
    {
        processingChain->convertFromJson(convertedProcessingChain);
    }
    else
    {
        // Reset to default.
        delete processingChain;
        processingChain = processingBlockFactory.createProcessingChain();
    }
}

std::string Patch::getObjectType() const
{
    return c_typeName;
}

void Patch::activate()
{
    processingChain->activate();
}

void Patch::deactivate()
{
    processingChain->deactivate();
}

void Patch::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    processingChain->execute(strip, noteToLightMap);
}

uint8_t Patch::getBank() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return bank;
}

void Patch::setBank(uint8_t bank)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->bank = bank;
}

bool Patch::hasBankAndProgram() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return bankAndProgramSet;
}

uint8_t Patch::getProgram() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return program;
}

void Patch::clearBankAndProgram()
{
    std::lock_guard<std::mutex> lock(mutex);

    bankAndProgramSet = false;
}

void Patch::setProgram(uint8_t program)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->program = program;
    bankAndProgramSet = true;
}

std::string Patch::getName() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return name;
}

void Patch::setName(const std::string name)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->name = name;
}
