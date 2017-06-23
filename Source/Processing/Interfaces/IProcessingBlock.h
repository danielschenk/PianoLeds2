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
