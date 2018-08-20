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

#include "Drivers/Arduino/ArduinoMidiInput.h"
#include "MidiTask.h"

#include "Processing/RgbFunctionFactory.h"
#include "Processing/ProcessingBlockFactory.h"
#include "Processing/Concert.h"
#include "Processing/Interfaces/IPatch.h"
#include "Processing/EqualRangeRgbSource.h"
#include "Processing/NoteRgbSource.h"
#include "Processing/LinearRgbFunction.h"

static LoggingTask* gs_pLoggingTask(nullptr);

static ArduinoMidiInput* gs_pMidiInput(nullptr);
static MidiTask* gs_pMidiTask(nullptr);

static RgbFunctionFactory* gs_pRgbFunctionFactory(nullptr);
static ProcessingBlockFactory* gs_pProcessingBlockFactory(nullptr);
static Processing::TNoteToLightMap* gs_pNoteToLightMap(nullptr);
static Concert* gs_pConcert(nullptr);

static constexpr uint32_t c_defaultStackSize(1024);

enum
{
    PRIORITY_IDLE = 0,
    PRIORITY_LOW = 1,
    PRIORITY_UI = 2,
    PRIORITY_CRITICAL = 3
};

static void initTask(void* pvParameters)
{
    // Initialize logging
    Serial.begin(115200);
    gs_pLoggingTask = new LoggingTask(Serial,
                                      c_defaultStackSize,
                                      PRIORITY_LOW);

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

    // Delete ourselves.
    vTaskDelete(NULL);
}

void setup()
{
    // Start the init task, which will initialize everything and delete itself.
    xTaskCreate(initTask,
                "init",
                c_defaultStackSize,
                nullptr,            // no parameters
                PRIORITY_CRITICAL,
                nullptr);           // no need to store task handle

    xPortStartScheduler();
}

void loop()
{
    // Nothing to do, leave everything to the OS.
}


// This function is called by the Arduino Serial driver
void serialEvent2()
{
    gs_pMidiTask->wake();
}
