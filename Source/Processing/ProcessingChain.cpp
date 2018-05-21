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
 */

#include <Common/Logging.h>
#include <Common/Utilities/Json11Helper.h>

#include "ProcessingChain.h"

#include "Interfaces/IProcessingBlock.h"
#include "Interfaces/IProcessingBlockFactory.h"

#define LOGGING_COMPONENT "ProcessingChain"

ProcessingChain::ProcessingChain(const IProcessingBlockFactory& rProcessingBlockFactory)
    : m_mutex()
    , m_rProcessingBlockFactory(rProcessingBlockFactory)
    , m_active()
    , m_processingChain()
{
}

ProcessingChain::~ProcessingChain()
{
    deleteProcessingBlocks();
}

void ProcessingChain::insertBlock(IProcessingBlock* pBlock, unsigned int index)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if(index > m_processingChain.size())
    {
        index = m_processingChain.size();
    }

    m_processingChain.insert(m_processingChain.begin() + index, pBlock);
    m_active ? pBlock->activate() : pBlock->deactivate();
}

void ProcessingChain::insertBlock(IProcessingBlock* pBlock)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_processingChain.insert(m_processingChain.end(), pBlock);
    m_active ? pBlock->activate() : pBlock->deactivate();
}

Json ProcessingChain::convertToJson() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    Json::object converted;
    converted[IProcessingBlock::c_objectTypeKey] = std::string(IProcessingBlock::c_typeNameProcessingChain);

    Json::array convertedChain;
    for(auto pProcessingBlock : m_processingChain)
    {
        convertedChain.push_back(pProcessingBlock->convertToJson());
    }
    converted[c_processingChainJsonKey] = convertedChain;

    return Json(converted);
}

void ProcessingChain::convertFromJson(const Json& rConverted)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    deleteProcessingBlocks();

    Json11Helper helper(__PRETTY_FUNCTION__, rConverted);
    Json::array convertedChain;
    if(helper.getItemIfPresent(c_processingChainJsonKey, convertedChain))
    {
        for(auto convertedBlock : convertedChain)
        {
            m_processingChain.push_back(m_rProcessingBlockFactory.createProcessingBlock(convertedBlock));
        }
    }
    else
    {
        LOG_ERROR("convertFromJson: JSON does not contain list of processing blocks. Chain will stay empty.");
    }

    updateAllBlockStates();
}

std::string ProcessingChain::getObjectType() const
{
    return IProcessingBlock::c_typeNameProcessingChain;
}

void ProcessingChain::activate()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    for(auto pProcessingBlock : m_processingChain)
    {
        pProcessingBlock->activate();
    }

    m_active = true;
}

void ProcessingChain::deactivate()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    for(auto pProcessingBlock : m_processingChain)
    {
        pProcessingBlock->deactivate();
    }

    m_active = false;
}

void ProcessingChain::execute(Processing::TRgbStrip& strip)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    for(auto pProcessingBlock : m_processingChain)
    {
        pProcessingBlock->execute(strip);
    }
}

void ProcessingChain::deleteProcessingBlocks()
{
    for(auto pProcessingBlock : m_processingChain)
    {
        delete pProcessingBlock;
    }
    m_processingChain.clear();
}

void ProcessingChain::updateAllBlockStates()
{
    if(m_active)
    {
        for(auto pProcessingBlock : m_processingChain)
        {
            pProcessingBlock->activate();
        }
    }
    else
    {
        for(auto pProcessingBlock : m_processingChain)
        {
            pProcessingBlock->deactivate();
        }
    }
}
