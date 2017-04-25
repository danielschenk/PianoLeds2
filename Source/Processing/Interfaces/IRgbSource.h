/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Interface for RGB sources.
 */

#ifndef PROCESSING_IRGBSOURCE_H_
#define PROCESSING_IRGBSOURCE_H_

#include <vector>

#include "ProcessingTypes.h"

/**
 * Interface for RGB sources.
 */
class IRgbSource
{
public:
    /**
     * Destructor.
     */
    virtual ~IRgbSource()
    {
    };

    /**
     * Execute this source and write output to the specified buffer.
     *
     * @param   [out]   output  The output buffer to write to.
     */
    virtual void execute(Processing::TRgbStrip& output) = 0;
};

#endif /* PROCESSING_IRGBSOURCE_H_ */
