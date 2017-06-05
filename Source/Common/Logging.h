/**
 * @file
 * @copyright (c) Daniel Schenk, 2017
 * This file is part of MLC2.
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
