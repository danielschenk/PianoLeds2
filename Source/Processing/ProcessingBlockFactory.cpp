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

#include "ProcessingBlockFactory.h"
#include "NoteRgbSource.h"
#include "EqualRangeRgbSource.h"
#include "ProcessingChain.h"
#include "Patch.h"

ProcessingBlockFactory::ProcessingBlockFactory(IMidiInput& rMidiInput,
                                               const Processing::TNoteToLightMap& rNoteToLightMap,
                                               const IRgbFunctionFactory& rRgbFunctionFactory)
    : m_rMidiInput(rMidiInput)
    , m_rNoteToLightMap(rNoteToLightMap)
    , m_rRgbFunctionFactory(rRgbFunctionFactory)
{
}

ProcessingBlockFactory::~ProcessingBlockFactory()
{
}

IProcessingBlock* ProcessingBlockFactory::createProcessingBlock(json json) const
{
    IProcessingBlock* processingBlock = nullptr;
    if(json.count(IJsonConvertible::c_objectTypeKey) > 0)
    {
        std::string objectType = json[IJsonConvertible::c_objectTypeKey];
        if(objectType == IProcessingBlock::c_typeNameEqualRangeRgbSource)
        {
            processingBlock = new EqualRangeRgbSource();
        }
        else if(objectType == IProcessingBlock::c_typeNameNoteRgbSource)
        {
            processingBlock = new NoteRgbSource(m_rMidiInput, m_rNoteToLightMap, m_rRgbFunctionFactory);
        }
        else if(objectType == IProcessingBlock::c_typeNameProcessingChain)
        {
            // A processing chain needs the factory to construct its children
            processingBlock = new ProcessingChain(*this);
        }
        else if(objectType == IProcessingBlock::c_typeNamePatch)
        {
            // A patch needs the factory to construct its children
            processingBlock = new Patch(*this);
        }

        if(processingBlock != nullptr)
        {
            processingBlock->convertFromJson(json);
        }
    }

    return processingBlock;
}
