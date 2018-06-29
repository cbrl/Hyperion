#pragma once

#include "log/spdlog/spdlog.h"
#include <iostream>


using LogLevel = spdlog::level::level_enum;

class Logger final {
private:
	Logger() {
		try {
			file    = spdlog::rotating_logger_mt("file_log", "log.txt", 1024 * 1024, 1);
			console = spdlog::stdout_color_mt("console_log");
		}
		catch (const spdlog::spdlog_ex& ex) {
			std::cout << "Log initialization failed: " << ex.what() << std::endl;
		}
	}

	~Logger() {
		file.reset();
		console.reset();
	}


public:
	Logger(const Logger& logger) = delete;
	Logger(Logger&& logger) = delete;

	Logger& operator=(const Logger& logger) = delete;
	Logger& operator=(Logger&& logger) = delete;


public:
	//----------------------------------------------------------------------------------
	// Log all
	//----------------------------------------------------------------------------------

	// Log a message with both loggers
	template<typename T>
	static void log(LogLevel level, const T& msg) {
		auto& instance = get();
		instance.file->log(level, msg);
		instance.console->log(level, msg);
	}

	// Log a message with both loggers
	template<typename Arg1, typename... Args>
	static void log(LogLevel level,
	                const char* fmt,
	                const Arg1& arg1,
	                Args&&... args) {

		auto& instance = get();
		instance.file->log(level, fmt, arg1, std::forward<Args>(args)...);
		instance.console->log(level, fmt, arg1, std::forward<Args>(args)...);
	}


	//----------------------------------------------------------------------------------
	// Console Log
	//----------------------------------------------------------------------------------

	// Log a message with the default console logger
	template<typename T>
	static void logConsole(LogLevel level, const T& msg) {
		get().console->log(level, msg);
	}

	// Log a message with the default console logger
	template<typename Arg1, typename... ArgsT>
	static void logConsole(LogLevel level,
	                       const char* fmt,
	                       const Arg1& arg1,
	                       ArgsT&&... args) {

		get().console->log(level, fmt, arg1, std::forward<ArgsT>(args)...);
	}


	//----------------------------------------------------------------------------------
	// File Log
	//----------------------------------------------------------------------------------

	// Log a message with the default file logger
	template<typename T>
	static void logFile(LogLevel level, const T& msg) {
		get().file->log(level, msg);
	}

	// Log a message with the default file logger
	template<typename Arg1, typename... ArgsT>
	static void logFile(LogLevel level,
	                    const char* fmt,
	                    const Arg1& arg1,
	                    ArgsT&&... args) {

		get().file->log(level, fmt, arg1, std::forward<ArgsT>(args)...);
	}


private:
	static Logger& get() {
		static Logger instance;
		return instance;
	}


private:
	std::shared_ptr<spdlog::logger> file;
	std::shared_ptr<spdlog::logger> console;
};
