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

#include "Common/Utilities/JsonHelper.h"
#include "Interfaces/IProcessingBlockFactory.h"

#include "Concert.h"
#include "Patch.h"

Concert::Concert(IMidiInput& rMidiInput, IProcessingBlockFactory& rProcessingBlockFactory)
    : m_noteToLightMap()
    , m_patches()
    , m_activePatch(m_patches.end())
    , m_listeningToProgramChange(false)
    , m_programChangeChannel(0)
    , m_currentBank(0)
    , m_rMidiInput(rMidiInput)
    , m_rProcessingBlockFactory(rProcessingBlockFactory)
    , m_scheduler()
    , m_mutex()
{
    m_controlChangeSubscription = m_rMidiInput.subscribeControlChange(
        std::bind(&Concert::onControlChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    );
    m_programChangeSubscription = m_rMidiInput.subscribeProgramChange(
        std::bind(&Concert::onProgramChange, this, std::placeholders::_1, std::placeholders::_2)
    );
}

Concert::~Concert()
{
    m_rMidiInput.unsubscribeProgramChange(m_programChangeSubscription);
    m_rMidiInput.unsubscribeControlChange(m_controlChangeSubscription);

    for(auto pPatch : m_patches)
    {
        delete pPatch;
    }
}

IPatch& Concert::addPatch()
{
    IPatch* pPatch = m_rProcessingBlockFactory.createPatch();
    m_patches.push_back(pPatch);

    return *pPatch;
}

json Concert::convertToJson() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    json converted;
    converted[IJsonConvertible::c_objectTypeKey] = std::string(IProcessingBlock::c_typeNameConcert);
    converted[c_isListeningToProgramChangeJsonKey] = m_listeningToProgramChange;
    converted[c_programChangeChannelJsonKey] = m_programChangeChannel;
    converted[c_currentBankJsonKey] = m_currentBank;
    converted[c_noteToLightMapJsonKey] = Processing::convert(m_noteToLightMap);

    std::list<json> convertedPatches;
    for(const IPatch* pPatch : m_patches)
    {
        convertedPatches.push_back(pPatch->convertToJson());
    }
    converted[c_patchesJsonKey] = convertedPatches;

    return converted;
}

void Concert::convertFromJson(json converted)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    JsonHelper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(c_isListeningToProgramChangeJsonKey, m_listeningToProgramChange);
    helper.getItemIfPresent(c_programChangeChannelJsonKey, m_programChangeChannel);
    helper.getItemIfPresent(c_currentBankJsonKey, m_currentBank);
    
    Processing::TStringNoteToLightMap temp;
    if(helper.getItemIfPresent(c_noteToLightMapJsonKey, temp))
    {
        m_noteToLightMap = Processing::convert(temp);
    }

    for(IPatch* pPatch : m_patches)
    {
        delete pPatch;
    }
    m_patches.clear();
    for(json convertedPatch : converted[c_patchesJsonKey])
    {
        m_patches.push_back(m_rProcessingBlockFactory.createPatch(convertedPatch));
    }
}

bool Concert::isListeningToProgramChange() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    return m_listeningToProgramChange;
}

void Concert::setListeningToProgramChange(bool listeningToProgramChange)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_listeningToProgramChange = listeningToProgramChange;
}

Processing::TNoteToLightMap Concert::getNoteToLightMap() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    return m_noteToLightMap;
}

void Concert::setNoteToLightMap(Processing::TNoteToLightMap noteToLightMap)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_noteToLightMap = noteToLightMap;
}

uint8_t Concert::getProgramChangeChannel() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    return m_programChangeChannel;
}

void Concert::setProgramChangeChannel(uint8_t programChangeChannel)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_programChangeChannel = programChangeChannel;
}

uint16_t Concert::getCurrentBank() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    return m_currentBank;
}

void Concert::setCurrentBank(uint16_t bank)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_currentBank = bank;
}

void Concert::execute()
{
    m_scheduler.executeAll();
    // TODO execute active patch
}

void Concert::onProgramChange(uint8_t channel, uint8_t program)
{
    auto taskFn = [=]() {
        std::lock_guard<std::mutex> lock(m_mutex);

        if(channel != m_programChangeChannel)
        {
            return;
        }

        for(auto patchIt = m_patches.begin(); patchIt != m_patches.end(); ++patchIt)
        {
            IPatch* pPatch = *patchIt;
            if(pPatch->hasBankAndProgram())
            {
                if(pPatch->getBank() == m_currentBank)
                {
                    if(pPatch->getProgram() == program)
                    {
                        // Found a patch which matches the received program number and active bank.
                        if(m_activePatch != m_patches.end())
                        {
                            (*m_activePatch)->deactivate();
                        }
                        pPatch->activate();
                        m_activePatch = patchIt;
                    }
                }
            }
        }
    };
    m_scheduler.schedule(taskFn);
}

void Concert::onControlChange(uint8_t channel, IMidiInterface::TControllerNumber controllerNumber, uint8_t value)
{
    if((controllerNumber != IMidiInterface::BANK_SELECT_MSB) && (controllerNumber != IMidiInterface::BANK_SELECT_LSB))
    {
        return;
    }
    
    auto taskFn = [=]() {
        std::lock_guard<std::mutex> lock(m_mutex);

        if(channel != m_programChangeChannel)
        {
            return;
        }

        if(controllerNumber == IMidiInterface::BANK_SELECT_MSB)
        {
            m_currentBank = ((value << 8) | (m_currentBank & 0xff));
        }
        else if(controllerNumber == IMidiInterface::BANK_SELECT_LSB)
        {
            m_currentBank = ((m_currentBank & 0xff00) | value);
        }
    };
    m_scheduler.schedule(taskFn);
}
