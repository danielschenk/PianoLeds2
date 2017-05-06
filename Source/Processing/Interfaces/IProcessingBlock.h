/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Interface for processing blocks.
 */

#ifndef PROCESSING_IPROCESSINGBLOCK_H_
#define PROCESSING_IPROCESSINGBLOCK_H_

#include <vector>

#include "ProcessingTypes.h"
#include "IJsonConvertible.h"

/**
 * Interface for processing blocks.
 */
class IProcessingBlock
    : public IJsonConvertible
{
public:
    static constexpr const char* c_typeNameEqualRangeRgbSource  = "EqualRangeRgbSource";
    static constexpr const char* c_typeNameNoteRgbSource        = "NoteRgbSource";
    static constexpr const char* c_typeNameProcessingChain      = "ProcessingChain";

    /**
     * Destructor.
     */
    virtual ~IProcessingBlock()
    {
    };

    /**
     * Execute this block on the given strip.
     *
     * @param   [in/out]    strip   The strip to operate on.
     */
    virtual void execute(Processing::TRgbStrip& strip) = 0;
};

#endif /* PROCESSING_IPROCESSINGBLOCK_H_ */
