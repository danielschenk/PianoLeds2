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
 */

#include <list>

#include "Common/Utilities/Json11Helper.h"
#include "Interfaces/IProcessingBlockFactory.h"

#include "Concert.h"
#include "Interfaces/IPatch.h"

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

size_t Concert::size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_patches.size();
}

Concert::TPatchPosition Concert::addPatch()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    IPatch* pPatch = m_rProcessingBlockFactory.createPatch();
    if(pPatch == nullptr)
    {
        return c_invalidPatchPosition;
    }

    m_patches.push_back(pPatch);
    return m_patches.size() - 1;
}

IPatch* Concert::getPatch(TPatchPosition position) const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(position >= m_patches.size())
    {
        return nullptr;
    }

    return m_patches.at(position);
}

bool Concert::removePatch(TPatchPosition position)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(position >= m_patches.size())
    {
        return false;
    }

    m_patches.erase(m_patches.begin() + position);
    return true;
}

Json Concert::convertToJson() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    Json::object converted;
    converted[IJsonConvertible::c_objectTypeKey] = getObjectType();
    converted[c_isListeningToProgramChangeJsonKey] = m_listeningToProgramChange;
    converted[c_programChangeChannelJsonKey] = m_programChangeChannel;
    converted[c_currentBankJsonKey] = m_currentBank;
    converted[c_noteToLightMapJsonKey] = Processing::convert(m_noteToLightMap);

    Json::array convertedPatches;
    for(const IPatch* pPatch : m_patches)
    {
        convertedPatches.push_back(pPatch->convertToJson());
    }
    converted[c_patchesJsonKey] = convertedPatches;

    return Json(converted);
}

void Concert::convertFromJson(const Json& rConverted)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    Json11Helper helper(__PRETTY_FUNCTION__, rConverted);
    helper.getItemIfPresent(c_isListeningToProgramChangeJsonKey, m_listeningToProgramChange);
    helper.getItemIfPresent(c_programChangeChannelJsonKey, m_programChangeChannel);
    helper.getItemIfPresent(c_currentBankJsonKey, m_currentBank);
    
    Json::object convertedNoteToLightMap;
    if(helper.getItemIfPresent(c_noteToLightMapJsonKey, convertedNoteToLightMap))
    {
        m_noteToLightMap = Processing::convert(convertedNoteToLightMap);
    }

    for(IPatch* pPatch : m_patches)
    {
        delete pPatch;
    }
    m_patches.clear();

    Json::array convertedPatches;
    if(helper.getItemIfPresent(c_patchesJsonKey, convertedPatches))
    {
        for(const Json& rConvertedPatch : convertedPatches)
        {
            m_patches.push_back(m_rProcessingBlockFactory.createPatch(rConvertedPatch));
        }
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

std::string Concert::getObjectType() const
{
    return c_typeName;
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
