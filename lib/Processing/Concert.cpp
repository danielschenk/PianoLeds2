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
#include <cassert>

#include "Json11Helper.h"
#include "IProcessingBlockFactory.h"

#include "Concert.h"
#include "IPatch.h"

#define LOGGING_COMPONENT "Concert"

Concert::Concert(IMidiInput& midiInput, IProcessingBlockFactory& processingBlockFactory)
    : midiInput(midiInput)
    , processingBlockFactory(processingBlockFactory)
{
    midiInput.subscribe(*this);
}

Concert::~Concert()
{
    midiInput.unsubscribe(*this);

    for(auto patch : patches)
    {
        delete patch;
    }
}

size_t Concert::size() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return patches.size();
}

Concert::TPatchPosition Concert::addPatch()
{
    std::lock_guard<std::mutex> lock(mutex);

    IPatch* patch = processingBlockFactory.createPatch();
    assert(patch != nullptr);

    return addPatchInternal(patch);
}

Concert::TPatchPosition Concert::addPatch(IPatch* patch)
{
    std::lock_guard<std::mutex> lock(mutex);

    return addPatchInternal(patch);
}

Concert::TPatchPosition Concert::addPatchInternal(IPatch* patch)
{
    patches.push_back(patch);

    if(patches.size() == 1)
    {
        // First patch. Activate it.
        patch->activate();
        activePatchPosition = 0;
    }

    return patches.size() - 1;
}

IPatch* Concert::getPatch(TPatchPosition position) const
{
    std::lock_guard<std::mutex> lock(mutex);

    if(position >= patches.size())
    {
        return nullptr;
    }

    return patches.at(position);
}

bool Concert::removePatch(TPatchPosition position)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(position >= patches.size())
    {
        return false;
    }

    patches.erase(patches.begin() + position);
    return true;
}

Json Concert::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object converted;
    converted[IJsonConvertible::c_objectTypeKey] = getObjectType();
    converted[c_isListeningToProgramChangeJsonKey] = listeningToProgramChange;
    converted[c_programChangeChannelJsonKey] = programChangeChannel;
    converted[c_currentBankJsonKey] = currentBank;
    converted[c_noteToLightMapJsonKey] = Processing::convert(noteToLightMap);

    Json::array convertedPatches;
    for(const IPatch* patch : patches)
    {
        convertedPatches.push_back(patch->convertToJson());
    }
    converted[c_patchesJsonKey] = convertedPatches;

    return Json(converted);
}

void Concert::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);
    
    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(c_isListeningToProgramChangeJsonKey, listeningToProgramChange);
    helper.getItemIfPresent(c_programChangeChannelJsonKey, programChangeChannel);
    helper.getItemIfPresent(c_currentBankJsonKey, currentBank);
    
    Json::object convertedNoteToLightMap;
    if(helper.getItemIfPresent(c_noteToLightMapJsonKey, convertedNoteToLightMap))
    {
        noteToLightMap = Processing::convert(convertedNoteToLightMap);
        // Make sure all mapped lights fit into the strip
        createMinimumAmountOfLights();
    }

    for(IPatch* patch : patches)
    {
        delete patch;
    }
    patches.clear();

    Json::array convertedPatches;
    if(helper.getItemIfPresent(c_patchesJsonKey, convertedPatches))
    {
        for(const Json& convertedPatch : convertedPatches)
        {
            patches.push_back(processingBlockFactory.createPatch(convertedPatch));
        }
    }
}

bool Concert::isListeningToProgramChange() const
{
    std::lock_guard<std::mutex> lock(mutex);
    
    return listeningToProgramChange;
}

void Concert::setListeningToProgramChange(bool listeningToProgramChange)
{
    std::lock_guard<std::mutex> lock(mutex);
    
    this->listeningToProgramChange = listeningToProgramChange;
}

Processing::TNoteToLightMap Concert::getNoteToLightMap() const
{
    std::lock_guard<std::mutex> lock(mutex);
    
    return noteToLightMap;
}

void Concert::setNoteToLightMap(Processing::TNoteToLightMap noteToLightMap)
{
    std::lock_guard<std::mutex> lock(mutex);
    
    this->noteToLightMap = noteToLightMap;

    // Make sure all mapped lights fit into the strip
    createMinimumAmountOfLights();
}

void Concert::createMinimumAmountOfLights()
{
    uint16_t highestLightIndex(0);
    for(const auto& pair : noteToLightMap)
    {
        if(pair.second > highestLightIndex)
        {
            highestLightIndex = pair.second;
        }
    }

    size_t minimumAmount(highestLightIndex + 1);
    strip.reserve(minimumAmount);
    for(unsigned int i(0); i < minimumAmount; ++i)
    {
        strip.push_back(Processing::TRgb());
    }
}

size_t Concert::getStripSize() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return strip.size();
}

uint8_t Concert::getProgramChangeChannel() const
{
    std::lock_guard<std::mutex> lock(mutex);
    
    return programChangeChannel;
}

void Concert::setProgramChangeChannel(uint8_t programChangeChannel)
{
    std::lock_guard<std::mutex> lock(mutex);
    
    this->programChangeChannel = programChangeChannel;
}

uint16_t Concert::getCurrentBank() const
{
    std::lock_guard<std::mutex> lock(mutex);
    
    return currentBank;
}

void Concert::setCurrentBank(uint16_t bank)
{
    std::lock_guard<std::mutex> lock(mutex);
    
    currentBank = bank;
}

void Concert::execute()
{
    scheduler.executeAll();

    std::lock_guard<std::mutex> lock(mutex);

    if(activePatchPosition != c_invalidPatchPosition)
    {
        patches.at(activePatchPosition)->execute(strip, noteToLightMap);

        for(auto observer : observers)
        {
            observer->onStripUpdate(strip);
        }
    }
}

void Concert::subscribe(IObserver& observer)
{
    std::lock_guard<std::mutex> lock(mutex);
    observers.push_back(&observer);
}

void Concert::unsubscribe(IObserver& observer)
{
    std::lock_guard<std::mutex> lock(mutex);
    observers.remove(&observer);
}

std::string Concert::getObjectType() const
{
    return c_typeName;
}

void Concert::onNoteChange(uint8_t channel, uint8_t number, uint8_t velocity, bool on)
{
    // ignore
}

void Concert::onProgramChange(uint8_t channel, uint8_t program)
{
    auto taskFn = [=]() {
        std::lock_guard<std::mutex> lock(mutex);

        if(channel != programChangeChannel)
        {
            return;
        }

        for(auto patchIt = patches.begin(); patchIt != patches.end(); ++patchIt)
        {
            IPatch* patch = *patchIt;
            if(patch->hasBankAndProgram())
            {
                if(patch->getBank() == currentBank)
                {
                    if(patch->getProgram() == program)
                    {
                        // Found a patch which matches the received program number and active bank.
                        if(activePatchPosition != c_invalidPatchPosition)
                        {
                            IPatch* activePatch(patches.at(activePatchPosition));
                            LOG_INFO_PARAMS("deactivating patch '%s'", activePatch->getName().c_str());
                            activePatch->deactivate();
                        }
                        LOG_INFO_PARAMS("activating patch '%s'", patch->getName().c_str());
                        patch->activate();
                        activePatchPosition = patchIt - patches.begin();
                    }
                }
            }
        }
    };
    scheduler.schedule(taskFn);
}

void Concert::onControlChange(uint8_t channel, IMidiInterface::TControllerNumber controllerNumber, uint8_t value)
{
    if((controllerNumber != IMidiInterface::BANK_SELECT_MSB) && (controllerNumber != IMidiInterface::BANK_SELECT_LSB))
    {
        return;
    }

    auto taskFn = [=]() {
        std::lock_guard<std::mutex> lock(mutex);

        if(channel != programChangeChannel)
        {
            return;
        }

        if(controllerNumber == IMidiInterface::BANK_SELECT_MSB)
        {
            currentBank = (value << 7) | (currentBank & 0x7f);
        }
        else if(controllerNumber == IMidiInterface::BANK_SELECT_LSB)
        {
            currentBank = (currentBank & 0x7f80) | value;
        }
    };
    scheduler.schedule(taskFn);
}

void Concert::onChannelPressureChange(uint8_t channel, uint8_t value)
{
    // ignore
}

void Concert::onPitchBendChange(uint8_t channel, uint16_t value)
{
    // ignore
}
