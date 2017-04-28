/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
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
    virtual void execute(Processing::TRgbStrip& output);
    virtual json convertToJson() const;
    virtual void convertFromJson(json json);

    /**
     * Get color.
     */
    Processing::TRgb getColor() const;

    /**
     * Set color.
     */
    void setColor(Processing::TRgb color);

private:
    static constexpr const char* c_rJsonKey = "r";
    static constexpr const char* c_gJsonKey = "g";
    static constexpr const char* c_bJsonKey = "b";

    /** Output color. */
    Processing::TRgb m_color;
};

#endif /* PROCESSING_EQUALRANGERGBSOURCE_H_ */
