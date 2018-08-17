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
 * @brief Interface for MIDI inputs/outputs.
 */

#ifndef DRIVERS_INTERFACES_IMIDIINTERFACE_H_
#define DRIVERS_INTERFACES_IMIDIINTERFACE_H_

#include <cstdint>

/**
 * Interface for MIDI inputs/outputs.
 */
class IMidiInterface
{
public:
    /** Status type. */
    enum TStatus : uint8_t
    {
        NOTE_OFF = 0x80,
        NOTE_ON = 0x90,
        CONTROL_CHANGE = 0xB0,
        PROGRAM_CHANGE = 0xC0,
    };

    /** Controller number type. */
    enum TControllerNumber : uint8_t
    {
        BANK_SELECT_MSB = 0x00,
        BANK_SELECT_LSB = 0x20,
        DAMPER_PEDAL = 0x40,
    };

    static constexpr unsigned int c_numNotes = 256;
    static constexpr unsigned int c_maxNoteNumber = 255;
    static constexpr unsigned int c_numVelocities = 256;
    static constexpr unsigned int c_maxVelocity = 255;

    /**
     * Destructor.
     */
    virtual ~IMidiInterface()
    {
    };

    /**
     * Get number of available ports.
     *
     * @return Number of available ports.
     */
    virtual unsigned int getPortCount() const = 0;

    /**
     * Open a port.
     *
     * @param   number  [in]    Port number to open.
     */
    virtual void openPort(int number) = 0;
};



#endif /* DRIVERS_INTERFACES_IMIDIINTERFACE_H_ */
