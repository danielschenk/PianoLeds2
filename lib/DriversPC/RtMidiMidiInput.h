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
 * @brief MIDI input implementation which uses RtMidi.
 */

#ifndef SOURCE_DRIVERS_SOURCE_RTMIDIMIDIINPUT_H_
#define SOURCE_DRIVERS_SOURCE_RTMIDIMIDIINPUT_H_

#include <BaseMidiInput.h>

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
    RtMidiIn* m_rtMidiIn;
};

#endif /* SOURCE_DRIVERS_SOURCE_RTMIDIMIDIINPUT_H_ */
