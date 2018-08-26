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
 * @brief RGB source which generates an equal range of colors.
 */

#ifndef PROCESSING_EQUALRANGERGBSOURCE_H_
#define PROCESSING_EQUALRANGERGBSOURCE_H_

#include "Interfaces/IProcessingBlock.h"

/**
 * RGB source which generates an equal range of colors.
 */
class EqualRangeRgbSource
    : public IProcessingBlock
{
public:
    /**
     * Constructor.
     */
    EqualRangeRgbSource();

    /**
     * Destructor.
     */
    virtual ~EqualRangeRgbSource();

    // Prevent implicit copy constructor and assignment operator.
    EqualRangeRgbSource(EqualRangeRgbSource&) = delete;
    EqualRangeRgbSource& operator=(EqualRangeRgbSource&) = delete;

    // IProcessingBlock implementation.
    virtual void activate();
    virtual void deactivate();
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap);
    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& converted);

    /**
     * Get color.
     */
    Processing::TRgb getColor() const;

    /**
     * Set color.
     */
    void setColor(Processing::TRgb color);

protected:
    // IProcessingBlock implementation
    virtual std::string getObjectType() const;

private:
    static constexpr const char* c_rJsonKey = "r";
    static constexpr const char* c_gJsonKey = "g";
    static constexpr const char* c_bJsonKey = "b";

    /** Mutex to protect the members. */
    mutable std::mutex m_mutex;

    /** Output color. */
    Processing::TRgb m_color;
};

#endif /* PROCESSING_EQUALRANGERGBSOURCE_H_ */
