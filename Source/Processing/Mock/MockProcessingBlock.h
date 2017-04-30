/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Mock processing block.
 */

#ifndef PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_
#define PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_

#include <gmock/gmock.h>

#include "../Interfaces/IProcessingBlock.h"

class MockProcessingBlock
    : public IProcessingBlock
{
public:
    // IProcessingBlock implementation
    MOCK_METHOD1(execute, void(Processing::TRgbStrip& strip));
    MOCK_CONST_METHOD0(convertToJson, json());
    MOCK_METHOD1(convertFromJson, void(json converted));
};

#endif /* PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_ */
