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