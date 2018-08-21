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
 *
 * The MLC2 application for the ESP32, using the Arduino core.
 */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "LoggingTask.h"
#include "Common/Logging.h"

#include "Drivers/Arduino/ArduinoMidiInput.h"
#include "MidiTask.h"

#include "Processing/RgbFunctionFactory.h"
#include "Processing/ProcessingBlockFactory.h"
#include "Processing/Concert.h"
#include "Processing/Interfaces/IPatch.h"
#include "Processing/EqualRangeRgbSource.h"
#include "Processing/NoteRgbSource.h"
#include "Processing/LinearRgbFunction.h"

#define LOGGING_COMPONENT "Esp32Application"

static LoggingTask* gs_pLoggingTask(nullptr);

static ArduinoMidiInput* gs_pMidiInput(nullptr);
static MidiTask* gs_pMidiTask(nullptr);

static RgbFunctionFactory* gs_pRgbFunctionFactory(nullptr);
static ProcessingBlockFactory* gs_pProcessingBlockFactory(nullptr);
static Processing::TNoteToLightMap* gs_pNoteToLightMap(nullptr);
static Concert* gs_pConcert(nullptr);

static constexpr uint32_t c_defaultStackSize(4096);

enum
{
    /**
     * Idle priority level.
     */
    PRIORITY_IDLE = 0,

    /**
     * Priority level for everything which is least important.
     * @note Same as the task which runs the setup/loop functions.
     * Examples: logging
     */
    PRIORITY_LOW = 1,

    /**
     * Priority level for things which are not critical, but should not be delayed by logging.
     * Examples: web UI for configuration
     * */
    PRIORITY_UI = 2,

    /**
     * Priority level for the main tasks of the device which should always have priority.
     * Examples: MIDI, processing, SPI
     */
    PRIORITY_CRITICAL = 3
};

void setup()
{
    // Initialize logging
    Serial.begin(115200);
    gs_pLoggingTask = new LoggingTask(Serial,
                                      c_defaultStackSize,
                                      PRIORITY_LOW);

    LOG_INFO("MIDI-LED-Controller (MLC) (c) Daniel Schenk, 2017");
    LOG_INFO("initializing application...");

    // Initialize MIDI, baud rate is 31.25k
    Serial2.begin(31250);

    gs_pMidiInput = new ArduinoMidiInput(Serial2);
    gs_pMidiTask = new MidiTask(*gs_pMidiInput,
                                c_defaultStackSize,
                                PRIORITY_CRITICAL);

    // Initialize concert dependencies.
    gs_pRgbFunctionFactory = new RgbFunctionFactory;

    gs_pNoteToLightMap = new Processing::TNoteToLightMap;
    // TODO read the note-to-light-map from storage
    // For now add something to test with.
    uint8_t lightNumber = 0;
    for(uint8_t noteNumber = 60 /* middle C */; noteNumber < 72; ++noteNumber)
    {
        (*gs_pNoteToLightMap)[noteNumber] = lightNumber;
        ++lightNumber;
    }

    gs_pProcessingBlockFactory = new ProcessingBlockFactory(*gs_pMidiInput,
                                                            *gs_pNoteToLightMap,
                                                            *gs_pRgbFunctionFactory);

    gs_pConcert = new Concert(*gs_pMidiInput,
                              *gs_pProcessingBlockFactory);

    // TODO read concert from storage
    // For now, add something to test with.
    IPatch* pPatch(gs_pConcert->getPatch(gs_pConcert->addPatch()));
    pPatch->setName("whiteOnBlue");
    pPatch->activate();

    // Add constant blue background
    EqualRangeRgbSource* pSrc1(new EqualRangeRgbSource);
    pSrc1->setColor(Processing::TRgb({0, 0, 255}));
    pPatch->getProcessingChain().insertBlock(pSrc1);

    // Full white for any sounding key
    NoteRgbSource* pSrc2(new NoteRgbSource(*gs_pMidiInput,
                                           *gs_pNoteToLightMap,
                                           *gs_pRgbFunctionFactory));
    LinearRgbFunction* pRgbFunction(new LinearRgbFunction({255, 0}, {255, 0}, {255, 0}));
    pSrc2->setRgbFunction(pRgbFunction);
    pSrc2->setUsingPedal(true);
    pPatch->getProcessingChain().insertBlock(pSrc2);

    LOG_INFO("initialization done");
}

void loop()
{
    // TODO toggle a run LED instead?
    LOG_INFO("still alive :-)");

    std::string json;
    gs_pConcert->convertToJson().dump(json);
    LOG_INFO(json.c_str());

    // Nothing to do, leave everything to the other tasks.
    vTaskDelay(10000);
}


// This function is called by the Arduino Serial driver
void serialEvent2()
{
    gs_pMidiTask->wake();
}
