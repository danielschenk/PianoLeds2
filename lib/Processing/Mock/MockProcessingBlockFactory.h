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
    MOCK_CONST_METHOD1(createProcessingBlock, IProcessingBlock*(const Json& converted));
    MOCK_CONST_METHOD0(createPatch, IPatch*());
    MOCK_CONST_METHOD1(createPatch, IPatch*(const Json& converted));
    MOCK_CONST_METHOD0(createProcessingChain, IProcessingChain*());
};


#endif /* PROCESSING_MOCK_MOCKPROCESSINGBLOCKFACTORY_H_ */
