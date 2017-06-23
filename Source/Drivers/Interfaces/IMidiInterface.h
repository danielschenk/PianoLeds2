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
 * @brief Interface for MIDI inputs/outputs.
 */

#ifndef DRIVERS_INTERFACES_IMIDIINTERFACE_H_
#define DRIVERS_INTERFACES_IMIDIINTERFACE_H_

/**
 * Interface for MIDI inputs/outputs.
 */
class IMidiInterface
{
public:
    /** Status type. */
    enum TStatus
    {
        NOTE_OFF = 0x80,
        NOTE_ON = 0x90,
        CONTROL_CHANGE = 0xB0,
        PROGRAM_CHANGE = 0xC0,
    };

    /** Controller number type. */
    enum TControllerNumber
    {
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
