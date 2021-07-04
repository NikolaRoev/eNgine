#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>

//======================================================================================================================================================

class Log {
private:
	static spdlog::logger* logger;

public:
	static void init() {
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_pattern("%^[%T %s:%# %!]: %v%$");

		logger = new spdlog::logger("Log", console_sink);
		logger->set_level(spdlog::level::trace);
	}

	static void deinit() { delete logger; }

	inline static spdlog::logger* get_logger() { return logger; }
};

inline spdlog::logger* Log::logger;

//======================================================================================================================================================