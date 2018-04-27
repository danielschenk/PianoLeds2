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
    virtual IPatch* createPatch() const;
    virtual IPatch* createPatch(json converted) const;

private:
    /** Reference to the MIDI input to pass to new blocks. */
    IMidiInput& m_rMidiInput;

    /** Reference to the note to light map to pass to new blocks. */
    const Processing::TNoteToLightMap& m_rNoteToLightMap;

    /** Reference to the RGB function factory to pass to new blocks. */
    const IRgbFunctionFactory& m_rRgbFunctionFactory;
};

#endif /* PROCESSING_PROCESSINGBLOCKFACTORY_H_ */
