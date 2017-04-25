/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief Interface for RGB functions.
 */

#ifndef PROCESSING_IRGBFUNCTION_H_
#define PROCESSING_IRGBFUNCTION_H_

#include "ProcessingTypes.h"

/**
 * Interface for RGB functions.
 */
class IRgbFunction
{
public:
    /**
     * Destructor.
     */
    virtual ~IRgbFunction()
    {
    };

    /**
     * Calculate the output color based on the given note state and current time.
     *
     * @param[in]   noteState   The note state.
     * @param[in]   currentTime The current time.
     *
     * @return  The output color.
     */
    virtual Processing::TRgb calculate(const Processing::TNoteState& noteState, Processing::TTime currentTime) const = 0;
};

#endif /* PROCESSING_IRGBFUNCTION_H_ */
