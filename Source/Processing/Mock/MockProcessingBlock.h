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
 * @brief Mock processing block.
 */

#ifndef PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_
#define PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_

#include <gmock/gmock.h>

#include "../Interfaces/IProcessingBlock.h"

class MockProcessingBlock
    : public virtual IProcessingBlock
{
public:
    // IProcessingBlock implementation
    MOCK_METHOD0(activate, void());
    MOCK_METHOD0(deactivate, void());
    MOCK_METHOD1(execute, void(Processing::TRgbStrip& strip));
    MOCK_CONST_METHOD0(convertToJson, json());
    MOCK_METHOD1(convertFromJson, void(json converted));
};

#endif /* PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_ */
