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

#include "ArduinoMidiInput.h"
#include "LoggingTask.h"
#include "Logging.h"
#include "MidiMessageLogger.h"

#include "MidiTask.h"

#include "RgbFunctionFactory.h"
#include "ProcessingBlockFactory.h"
#include "Concert.h"
#include "IPatch.h"
#include "EqualRangeRgbSource.h"
#include "NoteRgbSource.h"
#include "LinearRgbFunction.h"
#include "ProcessingTask.h"
#include "LedTask.h"

#include "Board.h"
#include "FreeRtosTime.h"

#define LOGGING_COMPONENT "Esp32Application"

static MidiTask* gs_midiTask(nullptr);

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
    // Initialize time
    auto freeRtosTime = new FreeRtosTime;

    // Initialize logging
    LoggingEntryPoint::setTime(freeRtosTime);
    Serial.begin(115200, SERIAL_8N1, DEBUG_RX_PIN, DEBUG_TX_PIN);
    new LoggingTask(Serial,
                    c_defaultStackSize,
                    PRIORITY_LOW);

    LOG_INFO("MIDI-LED-Controller (MLC) (c) Daniel Schenk, 2017");
    LOG_INFO("initializing application...");

    // Initialize run LED
    pinMode(RUN_LED_PIN, OUTPUT);
    // LED off during initialization
    digitalWrite(RUN_LED_PIN, 0);

    // Initialize MIDI, baud rate is 31.25k
    Serial2.begin(31250, SERIAL_8N1, MIDI_RX_PIN, MIDI_TX_PIN);

    auto midiInput = new ArduinoMidiInput(Serial2);
    gs_midiTask = new MidiTask(*midiInput,
                               c_defaultStackSize,
                               PRIORITY_CRITICAL);

    // Initialize printing of MIDI messages
    new MidiMessageLogger(*midiInput);

    // Initialize concert dependencies.
    auto rgbFunctionFactory = new RgbFunctionFactory;

    Processing::TNoteToLightMap noteToLightMap;
    uint8_t lightNumber = 0;
    for(uint8_t noteNumber = 48 /* C below middle C */; noteNumber < 72; ++noteNumber)
    {
        noteToLightMap[noteNumber] = lightNumber;
        ++lightNumber;
    }

    auto processingBlockFactory = new ProcessingBlockFactory(*midiInput,
                                                             *rgbFunctionFactory);

    auto concert = new Concert(*midiInput,
                               *processingBlockFactory);

    // TODO read concert from storage
    // For now, add something to test with.
    concert->setNoteToLightMap(noteToLightMap);
    IPatch* patch(concert->getPatch(concert->addPatch()));
    patch->setName("whiteOnBlue");
    patch->setBank(0);
    patch->setProgram(12);

    // Add constant blue background
    auto src1(new EqualRangeRgbSource);
    src1->setColor(Processing::TRgb({0, 0, 32}));
    patch->getProcessingChain().insertBlock(src1);

    // Full white for any sounding key
    auto src2(new NoteRgbSource(*midiInput,
                                *rgbFunctionFactory));
    auto rgbFunction(new LinearRgbFunction({255, 0}, {255, 0}, {255, 0}));
    src2->setRgbFunction(rgbFunction);
    src2->setUsingPedal(true);
    patch->getProcessingChain().insertBlock(src2);
    patch->activate();

    // Add another patch
    IPatch* patch2(concert->getPatch(concert->addPatch()));
    auto src3(new NoteRgbSource(*midiInput,
                                *rgbFunctionFactory));

    // Sounding notes become blue, intensity is the velocity of the note multiplied by 2
    src3->setRgbFunction(new LinearRgbFunction({0, 0}, {0, 0}, {2, 0}));
    src3->setUsingPedal(true);

    patch2->getProcessingChain().insertBlock(src3);
    patch2->setName("blueNote");
    patch2->setBank(0);
    patch2->setProgram(11);

    concert->setListeningToProgramChange(true);

    // Start processing
    new ProcessingTask(*concert,
                       c_defaultStackSize,
                       PRIORITY_CRITICAL);

    // Start LED output
    new LedTask(*concert,
                LED_DATA_PIN,
                LED_CLOCK_PIN,
                c_defaultStackSize,
                PRIORITY_CRITICAL);

    LOG_INFO("initialization done");
}

void loop()
{
    static unsigned s_loopCount(0);

    // Nothing to do, leave everything to the other tasks.
    vTaskDelay(1000);

    // Blink to indicate we're alive.
    digitalWrite(RUN_LED_PIN, !digitalRead(RUN_LED_PIN));

    if((s_loopCount % (5 * 60)) == 0)
    {
        LOG_INFO_PARAMS("free heap: %u", ESP.getFreeHeap());
    }

    ++s_loopCount;
}


// TODO This function is not called on ESP32... :-(
void serialEvent2()
{
    gs_midiTask->wake();
}
