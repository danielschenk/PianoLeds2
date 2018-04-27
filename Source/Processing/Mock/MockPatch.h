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
 * @brief Mock patch.
 */

#ifndef PROCESSING_MOCK_MOCKPATCH_H_
#define PROCESSING_MOCK_MOCKPATCH_H_

#include <gmock/gmock.h>

#include "../Interfaces/IPatch.h"
#include "MockProcessingBlock.h"

class MockPatch
    : public IPatch
    , public virtual MockProcessingBlock
{
public:
    MOCK_CONST_METHOD0(hasBankAndProgram, bool());
    MOCK_CONST_METHOD0(getBank, uint8_t());
    MOCK_METHOD1(setBank, void(uint8_t bank));
    MOCK_CONST_METHOD0(getProgram, uint8_t());
    MOCK_METHOD1(setProgram, void(uint8_t program));
    MOCK_METHOD0(clearBankAndProgram, void());
    MOCK_CONST_METHOD0(getName, std::string());
    MOCK_METHOD1(setName, void(std::string name));
};


#endif /* PROCESSING_MOCK_MOCKPATCH_H_ */
