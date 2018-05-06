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
