/**
 * @file
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
 * This file is part of mlc2.
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
