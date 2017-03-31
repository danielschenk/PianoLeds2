/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of mlc2.
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

int main()
{
    gs_pMidiInput = new RtMidiMidiInput();
    assert(gs_pMidiInput != nullptr);

    int numFoundInputs = gs_pMidiInput->getPortCount();
    std::cout << "Found " << numFoundInputs << " MIDI inputs.\n";

    if(numFoundInputs > 0)
    {
        gs_pMidiInput->subscribeNoteOnOff(&OnNoteReceived);
        gs_pMidiInput->openPort(0);
        std::cout << "Opened port 0, incoming notes will be printed to stdout. Type <q> <ENTER> to quit.\n";

        while(std::getchar() != 'q')
        {
            // Wait for input
        }

        return 0;
    }
    else
    {
        return 1;
    }
}

