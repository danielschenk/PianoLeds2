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
     * @param   [in]    rNoteToLightMap Reference to the note to light map.
     */
    NoteRgbSource(IMidiInput& rMidiDriver, const Processing::TNoteToLightMap& rNoteToLightMap);

    /**
     * Destructor.
     */
    virtual ~NoteRgbSource();

    // Prevent implicit copy constructor and assignment operator.
    NoteRgbSource(NoteRgbSource&) = delete;
    NoteRgbSource& operator=(NoteRgbSource&) = delete;

    // IRgbSource implementation.
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

    /**
     * Calculate output value for a single color.
     *
     * @param[in]   noteNumber  Note number.
     *
     * @return Output color for the given note.
     */
    Processing::TRgb calculateOutputValue(uint8_t noteNumber);

    /** Indicates whether pedal should be used. */
    bool m_usingPedal;

    /** Reference to the note to light map. */
    const Processing::TNoteToLightMap& m_rNoteToLightMap;

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
