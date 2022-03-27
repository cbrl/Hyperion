#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <iostream>

using LogLevel = spdlog::level::level_enum;

class Logger final {
private:
	Logger() noexcept {
		try {
			#ifdef _WIN32
			auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
			#else
			auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
			#endif

			auto file_sink    = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log.txt", 1024 * 1024, 1);

			const std::string pattern = "[%Y-%m-%d %H:%M:%S.%e] [%l] %v";
			console_sink->set_pattern(pattern);
			file_sink->set_pattern(pattern);

			logger = std::make_shared<spdlog::logger>("log", spdlog::sinks_init_list{console_sink, file_sink});

			#ifdef _DEBUG
			console_sink->set_level(LogLevel::debug);
			file_sink->set_level(LogLevel::debug);
			#endif

			logger->log(LogLevel::info, "<=========================START=========================>");
		}
		catch (const spdlog::spdlog_ex& ex) {
			std::cout << "Log initialization failed: " << ex.what() << std::endl;
		}
	}


public:
	Logger(const Logger& logger) = delete;
	Logger(Logger&& logger) = delete;

	~Logger() {
		if (logger) logger->log(LogLevel::info, "<==========================END==========================>\n");
	}

	Logger& operator=(const Logger& logger) = delete;
	Logger& operator=(Logger&& logger) = delete;


public:
	//----------------------------------------------------------------------------------
	// All Loggers
	//----------------------------------------------------------------------------------

	// Send a message to the logger
	template<typename T>
	static void log(LogLevel level, T&& msg) {
		auto& instance = get();
		if (instance.logger) instance.logger->log(level, std::forward<T>(msg));
	}

	// Send a message to the logger
	template<typename... ArgsT>
	static void log(LogLevel level, fmt::format_string<ArgsT...> fmt, ArgsT&&... args) {
		auto& instance = get();
		if (instance.logger) instance.logger->log(level, fmt, std::forward<ArgsT>(args)...);
	}

	// Set the maximum level to log
	static void setLevel(LogLevel level) {
		auto& instance = get();
		if (instance.logger) instance.logger->set_level(level);
	}


private:
	static Logger& get() {
		static Logger instance;
		return instance;
	}


private:
	std::shared_ptr<spdlog::logger> logger;
};
