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
