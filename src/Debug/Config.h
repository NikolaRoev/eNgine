#pragma once
#include "Log.h"

//======================================================================================================================================================

#ifdef _DEBUG
	#define LOG_INIT()			::Log::init()
	#define LOG_DEINIT()		::Log::deinit()
	#define LOG_TRACE(...)		SPDLOG_LOGGER_TRACE(::Log::get_logger(), __VA_ARGS__)
	#define LOG_INFO(...)		SPDLOG_LOGGER_INFO(::Log::get_logger(), __VA_ARGS__)
	#define LOG_WARN(...)		SPDLOG_LOGGER_WARN(::Log::get_logger(), __VA_ARGS__)
	#define LOG_ERROR(...)		SPDLOG_LOGGER_ERROR(::Log::get_logger(), __VA_ARGS__)
	#define LOG_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(::Log::get_logger(), __VA_ARGS__)
#else
	#define LOG_INIT()
	#define LOG_DEINIT()
	#define LOG_TRACE(...)
	#define LOG_INFO(...)
	#define LOG_WARN(...)
	#define LOG_ERROR(...)
	#define LOG_CRITICAL(...)
#endif

//======================================================================================================================================================