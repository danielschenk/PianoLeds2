/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Mock processing block factory.
 */

#ifndef PROCESSING_MOCK_MOCKPROCESSINGBLOCKFACTORY_H_
#define PROCESSING_MOCK_MOCKPROCESSINGBLOCKFACTORY_H_

#include <gmock/gmock.h>

#include "../Interfaces/IProcessingBlockFactory.h"

class MockProcessingBlockFactory
    : public IProcessingBlockFactory
{
public:
    MOCK_CONST_METHOD1(createProcessingBlock, IProcessingBlock*(json converted));
};


#endif /* PROCESSING_MOCK_MOCKPROCESSINGBLOCKFACTORY_H_ */
