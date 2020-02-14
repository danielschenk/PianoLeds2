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

#include <Logging.h>
#include <Json11Helper.h>

#include "ProcessingChain.h"

#include "IProcessingBlock.h"
#include "IProcessingBlockFactory.h"

#define LOGGING_COMPONENT "ProcessingChain"

ProcessingChain::ProcessingChain(const IProcessingBlockFactory& processingBlockFactory)
    : mutex()
    , processingBlockFactory(processingBlockFactory)
    , active()
    , processingChain()
{
}

ProcessingChain::~ProcessingChain()
{
    deleteProcessingBlocks();
}

void ProcessingChain::insertBlock(IProcessingBlock* block, unsigned int index)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(index > processingChain.size())
    {
        index = processingChain.size();
    }

    processingChain.insert(processingChain.begin() + index, block);
    active ? block->activate() : block->deactivate();
}

void ProcessingChain::insertBlock(IProcessingBlock* block)
{
    std::lock_guard<std::mutex> lock(mutex);

    processingChain.insert(processingChain.end(), block);
    active ? block->activate() : block->deactivate();
}

Json ProcessingChain::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object converted;
    converted[IProcessingBlock::c_objectTypeKey] = getObjectType();

    Json::array convertedChain;
    for(auto processingBlock : processingChain)
    {
        convertedChain.push_back(processingBlock->convertToJson());
    }
    converted[c_processingChainJsonKey] = convertedChain;

    return Json(converted);
}

void ProcessingChain::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    deleteProcessingBlocks();

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    Json::array convertedChain;
    if(helper.getItemIfPresent(c_processingChainJsonKey, convertedChain))
    {
        for(auto convertedBlock : convertedChain)
        {
            processingChain.push_back(processingBlockFactory.createProcessingBlock(convertedBlock));
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
    std::lock_guard<std::mutex> lock(mutex);

    for(auto processingBlock : processingChain)
    {
        processingBlock->activate();
    }

    active = true;
}

void ProcessingChain::deactivate()
{
    std::lock_guard<std::mutex> lock(mutex);

    for(auto processingBlock : processingChain)
    {
        processingBlock->deactivate();
    }

    active = false;
}

void ProcessingChain::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    std::lock_guard<std::mutex> lock(mutex);

    // Start clean
    for(auto& color : strip)
    {
        color.r = 0;
        color.g = 0;
        color.b = 0;
    }

    for(auto processingBlock : processingChain)
    {
        processingBlock->execute(strip, noteToLightMap);
    }
}

void ProcessingChain::deleteProcessingBlocks()
{
    for(auto processingBlock : processingChain)
    {
        delete processingBlock;
    }
    processingChain.clear();
}

void ProcessingChain::updateAllBlockStates()
{
    if(active)
    {
        for(auto processingBlock : processingChain)
        {
            processingBlock->activate();
        }
    }
    else
    {
        for(auto processingBlock : processingChain)
        {
            processingBlock->deactivate();
        }
    }
}
