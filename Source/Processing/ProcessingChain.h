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
 * @brief Class for processing chains.
 */

#ifndef PROCESSING_PROCESSINGCHAIN_H_
#define PROCESSING_PROCESSINGCHAIN_H_

#include <mutex>
#include <list>

#include "Interfaces/IProcessingBlock.h"

class IProcessingBlockFactory;

/**
 * A processing chain which can hold multiple processing blocks connected in series.
 */
class ProcessingChain
    : public virtual IProcessingBlock
{
public:
    /**
     * Constructor.
     */
    ProcessingChain(const IProcessingBlockFactory& rProcessingBlockFactory);

    /**
     * Destructor.
     */
    virtual ~ProcessingChain();

    // Prevent implicit constructor, copy constructor and assignment operator.
    ProcessingChain() = delete;
    ProcessingChain(const ProcessingChain&) = delete;
    ProcessingChain& operator=(const ProcessingChain&) = delete;

    /**
     * Insert a processing block.
     *
     * @param[in]   pBlock  Pointer to the processing block.
     * @param[in]   index   Index in the chain at which the block should be inserted.
     */
    void insertBlock(IProcessingBlock* pBlock, unsigned int index);

    /**
     * Insert a processing block at the end.
     *
     * @param[in]   pBlock  Pointer to the processing block.
     */
    void insertBlock(IProcessingBlock* pBlock);

    // IProcessingBlock implementation
    virtual void activate();
    virtual void deactivate();
    virtual void execute(Processing::TRgbStrip& strip);
    virtual json convertToJson() const;
    virtual void convertFromJson(json json);

protected:
    /** Mutex to protect the members. */
    mutable std::recursive_mutex m_mutex;

    /** Reference to the processing block factory. */
    const IProcessingBlockFactory& m_rProcessingBlockFactory;

private:
    static constexpr const char* c_processingChainJsonKey = "processingChain";

    /** Whether all blocks in the chain are active or not. */
    bool m_active;

    /** The processing chain. Using a vector for optimal traversal. */
    std::vector<IProcessingBlock*> m_processingChain;

    void deleteProcessingBlocks();

    /** Activates/deactivates every block in the chain, based on whether we're active or not. */
    void updateAllBlockStates();
};

#endif /* PROCESSING_PROCESSINGCHAIN_H_ */
