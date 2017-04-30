/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Factory for processing blocks.
 */

#ifndef PROCESSING_PROCESSINGBLOCKFACTORY_H_
#define PROCESSING_PROCESSINGBLOCKFACTORY_H_

#include <json.hpp>
#include "Interfaces/ProcessingTypes.h"
using json = nlohmann::json;

class IMidiInput;
class IProcessingBlock;

/**
 * Factory for processing blocks.
 */
class ProcessingBlockFactory
{
public:
    /**
     * Constructor.
     */
    ProcessingBlockFactory(IMidiInput& rMidiInput, const Processing::TNoteToLightMap& rNoteToLightMap);

    /**
     * Destructor.
     */
    virtual ~ProcessingBlockFactory();

    /**
     * Create a processing block from the given JSON.
     *
     * @param[in]   converted   JSON object containing the persistent properties.
     */
    IProcessingBlock* createProcessingBlock(json converted) const;

private:
    /** Reference to the MIDI input to pass to new blocks. */
    IMidiInput& m_rMidiInput;

    /** Reference to the note to light map to pass to new blocks. */
    const Processing::TNoteToLightMap& m_rNoteToLightMap;
};

#endif /* PROCESSING_PROCESSINGBLOCKFACTORY_H_ */
