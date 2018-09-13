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
 */

#ifndef ESP32APPLICATION_BOARD_H_
#define ESP32APPLICATION_BOARD_H_

#include "BoardOverride.h"

#ifndef RUN_LED_PIN
#define RUN_LED_PIN         2
#endif

#ifndef DEBUG_RX_PIN
#define DEBUG_RX_PIN        3
#endif

#ifndef DEBUG_TX_PIN
#define DEBUG_TX_PIN        1
#endif

#ifndef MIDI_RX_PIN
#define MIDI_RX_PIN         16
#endif

#ifndef MIDI_TX_PIN
#define MIDI_TX_PIN         17
#endif

// TODO find out why hardware SPI hangs the Adafruit driver.
#ifndef LED_DATA_PIN
#define LED_DATA_PIN        13
#endif

#ifndef LED_CLOCK_PIN
#define LED_CLOCK_PIN       14
#endif

#endif /* ESP32APPLICATION_BOARD_H_ */
