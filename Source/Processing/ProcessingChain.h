/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Class for processing chains.
 */

#ifndef PROCESSING_PROCESSINGCHAIN_H_
#define PROCESSING_PROCESSINGCHAIN_H_

#include <list>

#include "Interfaces/IProcessingBlock.h"

class ProcessingBlockFactory;

/**
 * A processing chain which can hold multiple processing blocks connected in series.
 */
class ProcessingChain
    : public IProcessingBlock
{
public:
    /**
     * Constructor.
     */
    ProcessingChain(const ProcessingBlockFactory& rProcessingBlockFactory);

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
    virtual void execute(Processing::TRgbStrip& strip);
    virtual json convertToJson() const;
    virtual void convertFromJson(json json);

private:
    static constexpr const char* c_processingChainJsonKey = "processingChain";

    /** The processing chain. Using a vector for optimal traversal. */
    std::vector<IProcessingBlock*> m_processingChain;

    /** Reference to the processing block factory. */
    const ProcessingBlockFactory& m_rProcessingBlockFactory;

    void deleteProcessingBlocks();
};

#endif /* PROCESSING_PROCESSINGCHAIN_H_ */
