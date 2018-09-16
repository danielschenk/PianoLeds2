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

#include <Json11Helper.h>

#include "ProcessingBlockFactory.h"
#include "NoteRgbSource.h"
#include "EqualRangeRgbSource.h"
#include "ProcessingChain.h"
#include "Patch.h"

ProcessingBlockFactory::ProcessingBlockFactory(IMidiInput& midiInput,
                                               const IRgbFunctionFactory& rgbFunctionFactory)
    : m_midiInput(midiInput)
    , m_rgbFunctionFactory(rgbFunctionFactory)
{
}

ProcessingBlockFactory::~ProcessingBlockFactory()
{
}

IProcessingBlock* ProcessingBlockFactory::createProcessingBlock(const Json& converted) const
{
    IProcessingBlock* processingBlock = nullptr;

    Json11Helper helper(__PRETTY_FUNCTION__, converted);

    std::string objectType;
    if(helper.getItemIfPresent(IJsonConvertible::c_objectTypeKey, objectType))
    {
        if(objectType == IProcessingBlock::c_typeNameEqualRangeRgbSource)
        {
            processingBlock = new EqualRangeRgbSource();
        }
        else if(objectType == IProcessingBlock::c_typeNameNoteRgbSource)
        {
            processingBlock = new NoteRgbSource(m_midiInput, m_rgbFunctionFactory);
        }
        else if(objectType == IProcessingBlock::c_typeNameProcessingChain)
        {
            // A processing chain needs the factory to construct its children
            processingBlock = new ProcessingChain(*this);
        }

        if(processingBlock != nullptr)
        {
            processingBlock->convertFromJson(converted);
        }
    }

    return processingBlock;
}

IPatch* ProcessingBlockFactory::createPatch() const
{
    // A patch needs the factory to construct its children
    return new Patch(*this);
}

IPatch* ProcessingBlockFactory::createPatch(const Json& converted) const
{
    IPatch* patch = createPatch();

    if(patch != nullptr)
    {
        patch->convertFromJson(converted);
    }

    return patch;
}

IProcessingChain* ProcessingBlockFactory::createProcessingChain() const
{
    // A patch needs the factory to construct its children
    return new ProcessingChain(*this);
}
