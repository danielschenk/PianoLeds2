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

    /** Mutex to protect the members. */
    mutable std::mutex m_mutex;

    /** Output color. */
    Processing::TRgb m_color;
};

#endif /* PROCESSING_EQUALRANGERGBSOURCE_H_ */
