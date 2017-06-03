/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 */

#include "ProcessingBlockFactory.h"
#include "NoteRgbSource.h"
#include "EqualRangeRgbSource.h"
#include "ProcessingChain.h"

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

        if(processingBlock != nullptr)
        {
            processingBlock->convertFromJson(json);
        }
    }

    return processingBlock;
}
