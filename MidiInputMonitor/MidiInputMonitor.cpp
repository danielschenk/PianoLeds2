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
 * @brief Simple test program using RtMidiMidiInput which prints received messages to stdout.
 */

#include <iostream>
#include <cstdio>
#include <cassert>
#include <Drivers/PC/RtMidiMidiInput.h>

static RtMidiMidiInput* gs_midiInput = nullptr;

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
    gs_midiInput = new RtMidiMidiInput();
    assert(gs_midiInput != nullptr);

    int numFoundInputs = gs_midiInput->getPortCount();
    std::cout << "Found " << numFoundInputs << " MIDI inputs.\n";

    if(numFoundInputs > 0)
    {
        gs_midiInput->subscribeNoteOnOff(&OnNoteReceived);
        gs_midiInput->subscribeControlChange(&OnControlChangeReceived);
        gs_midiInput->subscribeProgramChange(&OnProgramChangeReceived);
        gs_midiInput->openPort(0);
        std::cout << "Opened port 0, incoming notes and control changes will be printed to stdout. Type <q> <ENTER> to quit.\n";

        while(std::getchar() != 'q')
        {
            // Wait for input
        }

        delete gs_midiInput;
        return 0;
    }
    else
    {
        delete gs_midiInput;
        return 1;
    }
}

