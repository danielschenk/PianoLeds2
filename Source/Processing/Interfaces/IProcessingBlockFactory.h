/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Interface for processing block factories.
 */

#ifndef PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_
#define PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_

#include <json.hpp>
using json = nlohmann::json;

class IProcessingBlock;

/**
 * Interface for processing block factories.
 */
class IProcessingBlockFactory
{
public:
    /**
     * Destructor.
     */
    virtual ~IProcessingBlockFactory()
    {
    }

    /**
     * Create a processing block from the given JSON.
     *
     * @param[in]   converted   JSON object containing the persistent properties.
     */
    virtual IProcessingBlock* createProcessingBlock(json converted) const = 0;
};


#endif /* PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_ */
