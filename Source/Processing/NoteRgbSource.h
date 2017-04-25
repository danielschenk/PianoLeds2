/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
 * 
 * @brief RGB source which generates RGB data based on note on/off events.
 */

#ifndef PROCESSING_NOTERGBSOURCE_H_
#define PROCESSING_NOTERGBSOURCE_H_

#include <array>

#include <Processing/Interfaces/IRgbSource.h>
#include <Drivers/Interfaces/IMidiInput.h>
#include <Common/Scheduler.h>

class IRgbFunction;

/**
 * RGB source which generates RGB data based on note on/off events.
 */
class NoteRgbSource
    : public IRgbSource
{
public:
    /**
     * Constructor.
     *
     * @param   [in]    rMidiInput      Reference to the MIDI input.
     */
    NoteRgbSource(IMidiInput& rMidiDriver);

    /**
     * Destructor.
     */
    virtual ~NoteRgbSource();

    // Prevent implicit copy constructor and assignment operator.
    NoteRgbSource(NoteRgbSource&) = delete;
    NoteRgbSource& operator=(NoteRgbSource&) = delete;

    // IRgbSource implementation.
    virtual void execute(Processing::TRgbStrip& output);

    uint8_t getChannel() const;
    void setChannel(uint8_t channel);
    bool isUsingPedal() const;
    void setUsingPedal(bool usingPedal);

    void setRgbFunction(IRgbFunction* pRgbFunction);
    void setNoteToLightMap(const Processing::TNoteToLightMap& noteToLightMap);

private:
    /**
     * Handler for note on/off events.
     */
    void handleNoteOnOff(uint8_t channel, uint8_t number, uint8_t velocity, bool on);

    /**
     * Handler for control change events.
     */
    void handleControlChange(uint8_t channel, IMidiInput::TControllerNumber number, uint8_t value);

    /**
     * Calculate output value for a single color.
     *
     * @param[in]   noteNumber  Note number.
     *
     * @return Output color for the given note.
     */
    Processing::TRgb calculateOutputValue(uint8_t noteNumber);

    /** Output color. */
    Processing::TRgb m_color;

    /** Indicates whether pedal should be used. */
    bool m_usingPedal;

    /** Note to light map. */
    Processing::TNoteToLightMap m_noteToLightMap;

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
