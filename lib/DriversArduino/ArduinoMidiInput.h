/**
 * @file
 *
 * MIT License
 * 
 * @copyright (c) 2018 Daniel Schenk <danielschenk@users.noreply.github.com>
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
 */

#ifndef DRIVERS_ARDUINO_ARDUINOMIDIINPUT_H_
#define DRIVERS_ARDUINO_ARDUINOMIDIINPUT_H_

#include "BaseMidiInput.h"

class Stream;

/**
 * MIDI input implementation which uses Arduino Serial.
 */
class ArduinoMidiInput
    : public BaseMidiInput
{
public:
    /**
     * Constructor.
     *
     * @param serial   The Arduino serial port driver to use.
     */
    explicit ArduinoMidiInput(Stream& serial);

    /**
     * Destructor.
     */
    virtual ~ArduinoMidiInput() = default;

    // Prevent implicit default constructors and assignment operator.
    ArduinoMidiInput() = delete;
    ArduinoMidiInput(const ArduinoMidiInput&) = delete;
    ArduinoMidiInput& operator=(const ArduinoMidiInput&) = delete;

    /**
     * Run the serial driver and process available data.
     */
    void run();

    // IMidiInterface implementation
    virtual unsigned int getPortCount() const;
    virtual void openPort(int number);

private:
    Stream& m_serial;
};

#endif /* DRIVERS_ARDUINO_ARDUINOMIDIINPUT_H_ */
