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
 * @brief RGB source which generates RGB data based on note on/off events.
 */

#ifndef PROCESSING_NOTERGBSOURCE_H_
#define PROCESSING_NOTERGBSOURCE_H_

#include <mutex>
#include <array>

#include <IMidiInput.h>
#include <Scheduler.h>
#include "IProcessingBlock.h"

class IRgbFunction;
class IRgbFunctionFactory;

/**
 * RGB source which generates RGB data based on note on/off events.
 */
class NoteRgbSource
    : public IProcessingBlock
    , public IMidiInput::IObserver
{
public:
    /**
     * Constructor.
     *
     * @param   [in]    midiInput           Reference to the MIDI input.
     * @param   [in]    rgbFunctionFactory  Reference to the RGB function factory.
     */
    NoteRgbSource(IMidiInput& midiDriver, const IRgbFunctionFactory& rgbFunctionFactory);

    /**
     * Destructor.
     */
    virtual ~NoteRgbSource();

    // Prevent implicit constructors and assignment operator.
    NoteRgbSource() = delete;
    NoteRgbSource(NoteRgbSource&) = delete;
    NoteRgbSource& operator=(NoteRgbSource&) = delete;

    // IProcessingBlock implementation.
    virtual void activate();
    virtual void deactivate();
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap);
    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& converted);

    uint8_t getChannel() const;
    void setChannel(uint8_t channel);
    bool isUsingPedal() const;
    void setUsingPedal(bool usingPedal);

    void setRgbFunction(IRgbFunction* rgbFunction);

    // IMidiInput::IObserver implementation
    virtual void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on);
    virtual void onControlChange(uint8_t channel, IMidiInput::TControllerNumber controller, uint8_t value);
    virtual void onProgramChange(uint8_t channel, uint8_t program);
    virtual void onChannelPressureChange(uint8_t channel, uint8_t value);
    virtual void onPitchBendChange(uint8_t channel, uint16_t value);


protected:
    // IProcessingBlock implementation
    virtual std::string getObjectType() const;

private:
    static constexpr const char* c_usingPedalJsonKey    = "usingPedal";
    static constexpr const char* c_channelJsonKey       = "channel";
    static constexpr const char* c_rgbFunctionJsonKey   = "rgbFunction";

    /** Mutex to protect the members. */
    mutable std::mutex m_mutex;

    /** Whether this block is active. */
    bool m_active;

    /** Indicates whether pedal should be used. */
    bool m_usingPedal;

    /** Reference to the RGB function factory. */
    const IRgbFunctionFactory& m_rgbFunctionFactory;

    /** Reference to the MIDI input. */
    IMidiInput& m_midiInput;

    /** MIDI channel to listen to. */
    uint8_t m_channel;

    /** Scheduler to decouple callbacks. */
    Scheduler m_scheduler;

    /** Actual note states. */
    std::array<Processing::TNoteState, IMidiInterface::c_numNotes> m_noteState;

    /** Actual pedal pressed state. */
    bool m_pedalPressed;

    /** Pointer to the RGB function. */
    IRgbFunction* m_rgbFunction;
};

#endif /* PROCESSING_NOTERGBSOURCE_H_ */
