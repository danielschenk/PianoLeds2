/**
 * @file
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 */

#include <Common/Logging.h>

#include "ProcessingChain.h"

#include "Interfaces/IProcessingBlock.h"
#include "Interfaces/IProcessingBlockFactory.h"

#define LOGGING_COMPONENT "ProcessingChain"

ProcessingChain::ProcessingChain(const IProcessingBlockFactory& rProcessingBlockFactory)
    : m_rProcessingBlockFactory(rProcessingBlockFactory)
    , m_processingChain()
{
}

ProcessingChain::~ProcessingChain()
{
    deleteProcessingBlocks();
}

void ProcessingChain::insertBlock(IProcessingBlock* pBlock, unsigned int index)
{
    if(index > m_processingChain.size())
    {
        index = m_processingChain.size();
    }

    m_processingChain.insert(m_processingChain.begin() + index, pBlock);
}

void ProcessingChain::insertBlock(IProcessingBlock* pBlock)
{
    m_processingChain.insert(m_processingChain.end(), pBlock);
}

json ProcessingChain::convertToJson() const
{
    json converted;
    converted[IProcessingBlock::c_objectTypeKey] = std::string(IProcessingBlock::c_typeNameProcessingChain);

    std::vector<json> convertedChain;
    for(auto pProcessingBlock : m_processingChain)
    {
        convertedChain.push_back(pProcessingBlock->convertToJson());
    }
    converted[c_processingChainJsonKey] = convertedChain;

    return converted;
}

void ProcessingChain::convertFromJson(json converted)
{
    deleteProcessingBlocks();

    if(converted.count(c_processingChainJsonKey) > 0)
    {
        std::vector<json> convertedChain = converted[c_processingChainJsonKey];
        for(auto convertedBlock : convertedChain)
        {
            m_processingChain.push_back(m_rProcessingBlockFactory.createProcessingBlock(convertedBlock));
        }
    }
    else
    {
        LOG_ERROR("convertFromJson: JSON does not contain list of processing blocks. Chain will stay empty.");
    }

    for(auto it = converted.begin(); it != converted.end(); ++it)
    {
        if(it.key() != c_processingChainJsonKey)
        {
            LOG_WARNING_PARAMS("convertFromJson: unknown key '%s'", it.key().c_str());
        }
    }
}

void ProcessingChain::execute(Processing::TRgbStrip& strip)
{
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
