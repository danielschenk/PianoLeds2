/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief RGB source which generates an equal range of colors.
 */

#ifndef PROCESSING_EQUALRANGERGBSOURCE_H_
#define PROCESSING_EQUALRANGERGBSOURCE_H_

#include <Processing/Interfaces/IRgbSource.h>

/**
 * RGB source which generates an equal range of colors.
 */
class EqualRangeRgbSource
    : public IRgbSource
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

    // IRgbSource implementation.
    virtual void execute(Processing::TRgbStrip& output);

    /**
     * Get color.
     */
    Processing::TRgb getColor() const;

    /**
     * Set color.
     */
    void setColor(Processing::TRgb color);

private:
    /** Output color. */
    Processing::TRgb m_color;
};

#endif /* PROCESSING_EQUALRANGERGBSOURCE_H_ */
