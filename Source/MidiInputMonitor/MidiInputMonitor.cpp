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
 * @brief Simple test program using RtMidiMidiInput which prints received messages to stdout.
 */

#include <iostream>
#include <cstdio>
#include <cassert>
#include <Drivers/PC/RtMidiMidiInput.h>

static RtMidiMidiInput* gs_pMidiInput = nullptr;

static void OnNoteReceived(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on)
{
    std::printf("%3s chan %2u pitch %3u vel %3u\n", on ? "ON" : "OFF", channel, pitch, velocity);
}

static void OnControlChangeReceived(uint8_t channel, IMidiInput::TControllerNumber controller, uint8_t value)
{
    std::printf("CON chan %2u controller %3u val %3u\n", channel, controller, value);
}

static void OnProgramChangeReceived(uint8_t channel, uint8_t number)
{
    std::printf("PRG chan %2u num %2u\n", channel, number);
}

int main()
{
    gs_pMidiInput = new RtMidiMidiInput();
    assert(gs_pMidiInput != nullptr);

    int numFoundInputs = gs_pMidiInput->getPortCount();
    std::cout << "Found " << numFoundInputs << " MIDI inputs.\n";

    if(numFoundInputs > 0)
    {
        gs_pMidiInput->subscribeNoteOnOff(&OnNoteReceived);
        gs_pMidiInput->subscribeControlChange(&OnControlChangeReceived);
        gs_pMidiInput->subscribeProgramChange(&OnProgramChangeReceived);
        gs_pMidiInput->openPort(0);
        std::cout << "Opened port 0, incoming notes and control changes will be printed to stdout. Type <q> <ENTER> to quit.\n";

        while(std::getchar() != 'q')
        {
            // Wait for input
        }

        delete gs_pMidiInput;
        return 0;
    }
    else
    {
        delete gs_pMidiInput;
        return 1;
    }
}

