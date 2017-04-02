/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of mlc2.
 * 
 * @brief MIDI input implementation which uses RtMidi.
 */

#ifndef SOURCE_DRIVERS_SOURCE_RTMIDIMIDIINPUT_H_
#define SOURCE_DRIVERS_SOURCE_RTMIDIMIDIINPUT_H_

#include <Drivers/Common/BaseMidiInput.h>

class RtMidiIn;

/**
 * MIDI input implementation which uses RtMidi.
 */
class RtMidiMidiInput
    : public BaseMidiInput
{
public:
    /**
     * Constructor.
     */
    RtMidiMidiInput();

    /**
     * Destructor.
     */
    virtual ~RtMidiMidiInput();

    // Prevent implicit copy constructor and assignment operator.
    RtMidiMidiInput(const RtMidiMidiInput&) = delete;
    RtMidiMidiInput& operator=(const RtMidiMidiInput&) = delete;

    // IMidiInterface implementation.
    virtual unsigned int getPortCount() const;
    virtual void openPort(int number);

private:
    /** The common callback for all instances. */
    static void RtMidiCommonCallback(double deltatime, std::vector<unsigned char> *message, void *userData);

    /** The instance-specific callback function handling all RtMidi callbacks and delegating them to subscribers. */
    void RtMidiCallback(double deltatime, std::vector<unsigned char> *message);

    /** Pointer to the RtMidiIn instance. */
    RtMidiIn* m_pRtMidiIn;
};

#endif /* SOURCE_DRIVERS_SOURCE_RTMIDIMIDIINPUT_H_ */
