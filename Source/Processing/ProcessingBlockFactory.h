/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Factory for processing blocks.
 */

#ifndef PROCESSING_PROCESSINGBLOCKFACTORY_H_
#define PROCESSING_PROCESSINGBLOCKFACTORY_H_

#include "Interfaces/ProcessingTypes.h"
#include "Interfaces/IProcessingBlockFactory.h"

class IMidiInput;
class IRgbFunctionFactory;

/**
 * Factory for processing blocks.
 */
class ProcessingBlockFactory
    : public IProcessingBlockFactory
{
public:
    /**
     * Constructor.
     */
    ProcessingBlockFactory(IMidiInput& rMidiInput, const Processing::TNoteToLightMap& rNoteToLightMap, const IRgbFunctionFactory& rRgbFunctionFactory);

    /**
     * Destructor.
     */
    virtual ~ProcessingBlockFactory();

    // IProcessingBlockFactory implementation
    virtual IProcessingBlock* createProcessingBlock(json converted) const;

private:
    /** Reference to the MIDI input to pass to new blocks. */
    IMidiInput& m_rMidiInput;

    /** Reference to the note to light map to pass to new blocks. */
    const Processing::TNoteToLightMap& m_rNoteToLightMap;

    /** Reference to the RGB function factory to pass to new blocks. */
    const IRgbFunctionFactory& m_rRgbFunctionFactory;
};

#endif /* PROCESSING_PROCESSINGBLOCKFACTORY_H_ */
