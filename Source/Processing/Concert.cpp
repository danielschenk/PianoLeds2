/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
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
 */

#include "Concert.h"

Concert::Concert(IMidiInput& rMidiInput, IProcessingBlockFactory& rProcessingBlockFactory)
    : m_noteToLightMap()
    , m_patches()
    , m_listeningToProgramChange(false)
    , m_programChangeChannel(0)
    , m_rMidiInput(rMidiInput)
    , m_rProcessingBlockFactory(rProcessingBlockFactory)
{
    // TODO subscribe to MIDI program changes
}

Concert::~Concert()
{
}

json Concert::convertToJson() const
{
    return json();
}

void Concert::convertFromJson(json json)
{
    // TODO
}

bool Concert::isListeningToProgramChange() const
{
    return m_listeningToProgramChange;
}

void Concert::setListeningToProgramChange(bool listeningToProgramChange)
{
    m_listeningToProgramChange = listeningToProgramChange;
}

Processing::TNoteToLightMap Concert::getNoteToLightMap() const
{
    return m_noteToLightMap;
}

void Concert::setNoteToLightMap(Processing::TNoteToLightMap noteToLightMap)
{
    m_noteToLightMap = noteToLightMap;
}

uint8_t Concert::getProgramChangeChannel() const
{
    return m_programChangeChannel;
}

void Concert::setProgramChangeChannel(uint8_t programChangeChannel)
{
    m_programChangeChannel = programChangeChannel;
}
