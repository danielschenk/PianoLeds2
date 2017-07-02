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
 * @brief RGB source which generates RGB data based on note on/off events.
 */

#ifndef PROCESSING_NOTERGBSOURCE_H_
#define PROCESSING_NOTERGBSOURCE_H_

#include <mutex>
#include <array>

#include <Drivers/Interfaces/IMidiInput.h>
#include <Common/Scheduler.h>
#include "Interfaces/IProcessingBlock.h"

class IRgbFunction;
class IRgbFunctionFactory;

/**
 * RGB source which generates RGB data based on note on/off events.
 */
class NoteRgbSource
    : public IProcessingBlock
{
public:
    /**
     * Constructor.
     *
     * @param   [in]    rMidiInput          Reference to the MIDI input.
     * @param   [in]    rNoteToLightMap     Reference to the note to light map.
     * @param   [in]    rRgbFunctionFactory Reference to the RGB function factory.
     */
    NoteRgbSource(IMidiInput& rMidiDriver, const Processing::TNoteToLightMap& rNoteToLightMap, const IRgbFunctionFactory& rRgbFunctionFactory);

    /**
     * Destructor.
     */
    virtual ~NoteRgbSource();

    // Prevent implicit copy constructor and assignment operator.
    NoteRgbSource(NoteRgbSource&) = delete;
    NoteRgbSource& operator=(NoteRgbSource&) = delete;

    // IProcessingBlock implementation.
    virtual void activate();
    virtual void deactivate();
    virtual void execute(Processing::TRgbStrip& output);
    virtual json convertToJson() const;
    virtual void convertFromJson(json json);

    uint8_t getChannel() const;
    void setChannel(uint8_t channel);
    bool isUsingPedal() const;
    void setUsingPedal(bool usingPedal);

    void setRgbFunction(IRgbFunction* pRgbFunction);

private:
    static constexpr const char* c_usingPedalJsonKey    = "usingPedal";
    static constexpr const char* c_channelJsonKey       = "channel";
    static constexpr const char* c_rgbFunctionJsonKey   = "rgbFunction";

    /**
     * Handler for note on/off events.
     */
    void handleNoteOnOff(uint8_t channel, uint8_t number, uint8_t velocity, bool on);

    /**
     * Handler for control change events.
     */
    void handleControlChange(uint8_t channel, IMidiInput::TControllerNumber number, uint8_t value);

    /** Mutex to protect the members. */
    mutable std::mutex m_mutex;

    /** Whether this block is active. */
    bool m_active;

    /** Indicates whether pedal should be used. */
    bool m_usingPedal;

    /** Reference to the note to light map. */
    const Processing::TNoteToLightMap& m_rNoteToLightMap;

    /** Reference to the RGB function factory. */
    const IRgbFunctionFactory& m_rRgbFunctionFactory;

    /** Reference to the MIDI input. */
    IMidiInput& m_rMidiInput;

    /** MIDI channel to listen to. */
    uint8_t m_channel;

    /** MIDI note on/off subscription. */
    IMidiInput::TSubscriptionToken m_noteOnOffSubscription;

    /** MIDI pedal event subscription. */
    IMidiInput::TSubscriptionToken m_controlChangeSubscription;

    /** Scheduler to decouple callbacks. */
    Scheduler m_scheduler;

    /** Actual note states. */
    std::array<Processing::TNoteState, IMidiInterface::c_numNotes> m_noteState;

    /** Actual pedal pressed state. */
    bool m_pedalPressed;

    /** Pointer to the RGB function. */
    IRgbFunction* m_pRgbFunction;
};

#endif /* PROCESSING_NOTERGBSOURCE_H_ */
