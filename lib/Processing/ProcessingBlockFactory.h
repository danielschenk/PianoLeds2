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
 * @brief Factory for processing blocks.
 */

#ifndef PROCESSING_PROCESSINGBLOCKFACTORY_H_
#define PROCESSING_PROCESSINGBLOCKFACTORY_H_

#include "ProcessingTypes.h"
#include "IProcessingBlockFactory.h"

class IMidiInput;
class IRgbFunctionFactory;
class ITime;

/**
 * Factory for processing blocks.
 */
class ProcessingBlockFactory
    : public IProcessingBlockFactory
{
public:
    /**
     * Constructor.
     */
    ProcessingBlockFactory(IMidiInput& midiInput,
                           const IRgbFunctionFactory& rgbFunctionFactory,
                           const ITime& time);

    // Prevent implicit constructors and assignment operator
    ProcessingBlockFactory() = delete;
    ProcessingBlockFactory(const ProcessingBlockFactory&) = delete;
    ProcessingBlockFactory& operator=(const ProcessingBlockFactory&) = delete;

    /**
     * Destructor.
     */
    virtual ~ProcessingBlockFactory();

    // IProcessingBlockFactory implementation
    virtual IProcessingBlock* createProcessingBlock(const Json& converted) const;
    virtual IPatch* createPatch() const;
    virtual IPatch* createPatch(const Json& converted) const;
    virtual IProcessingChain* createProcessingChain() const;

private:
    /** Reference to the MIDI input to pass to new blocks. */
    IMidiInput& m_midiInput;

    /** Reference to the RGB function factory to pass to new blocks. */
    const IRgbFunctionFactory& m_rgbFunctionFactory;

    /** Time provider to pass to new blocks. */
    const ITime& m_time;
};

#endif /* PROCESSING_PROCESSINGBLOCKFACTORY_H_ */
