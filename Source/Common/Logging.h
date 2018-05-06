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
 * @brief Logging macros.
 */

#ifndef COMMON_LOGGING_H_
#define COMMON_LOGGING_H_

#include "LoggingEntryPoint.h"

// TODO pass real time

#if ENABLE_LOG_ERROR
#define LOG_ERROR(fmt, ...) LoggingEntryPoint::logMessage(0, Logging::LogLevel_Error, LOGGING_COMPONENT, (fmt))
#define LOG_ERROR_PARAMS(fmt, ...) LoggingEntryPoint::logMessage(0, Logging::LogLevel_Error, LOGGING_COMPONENT, (fmt), __VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)
#define LOG_ERROR_PARAMS(fmt, ...)
#endif

#if ENABLE_LOG_WARNING
#define LOG_WARNING(fmt, ...) LoggingEntryPoint::logMessage(0, Logging::LogLevel_Warning, LOGGING_COMPONENT, (fmt))
#define LOG_WARNING_PARAMS(fmt, ...) LoggingEntryPoint::logMessage(0, Logging::LogLevel_Warning, LOGGING_COMPONENT, (fmt), __VA_ARGS__)
#else
#define LOG_WARNING(fmt, ...)
#define LOG_WARNING_PARAMS(fmt, ...)
#endif

#if ENABLE_LOG_INFO
#define LOG_INFO(fmt, ...) LoggingEntryPoint::logMessage(0, Logging::LogLevel_Info, LOGGING_COMPONENT, (fmt))
#define LOG_INFO_PARAMS(fmt, ...) LoggingEntryPoint::logMessage(0, Logging::LogLevel_Info, LOGGING_COMPONENT, (fmt), __VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)
#define LOG_INFO_PARAMS(fmt, ...)
#endif

#if ENABLE_LOG_DEBUG
#define LOG_DEBUG(fmt, ...) LoggingEntryPoint::logMessage(0, Logging::LogLevel_Debug, LOGGING_COMPONENT, (fmt))
#define LOG_DEBUG_PARAMS(fmt, ...) LoggingEntryPoint::logMessage(0, Logging::LogLevel_Debug, LOGGING_COMPONENT, (fmt), __VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#define LOG_DEBUG_PARAMS(fmt, ...)
#endif


#endif /* COMMON_LOGGING_H_ */
