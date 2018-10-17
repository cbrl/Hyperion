#pragma once

#include "log/spdlog/spdlog.h"
#include <iostream>


using LogLevel = spdlog::level::level_enum;

class Logger final {
private:
	Logger() noexcept {
		try {
			file    = spdlog::rotating_logger_mt("file_log", "log.txt", 1024 * 1024, 1);
			console = spdlog::stdout_logger_mt("console_log");

			#ifdef _DEBUG
			file->set_level(LogLevel::debug);
			console->set_level(LogLevel::debug);
			#endif

			file->log(LogLevel::info, "<=========================START=========================>");
		}
		catch (const spdlog::spdlog_ex& ex) {
			std::cout << "Log initialization failed: " << ex.what() << std::endl;
		}
	}


public:
	Logger(const Logger& logger) = delete;
	Logger(Logger&& logger) = delete;

	~Logger() {
		file->log(LogLevel::info, "<==========================END==========================>\n");
	}

	Logger& operator=(const Logger& logger) = delete;
	Logger& operator=(Logger&& logger) = delete;


public:
	//----------------------------------------------------------------------------------
	// All Loggers
	//----------------------------------------------------------------------------------

	// Log a message with both loggers
	template<typename T>
	static void log(LogLevel level, const T& msg) {
		auto& instance = get();
		if (instance.file) instance.file->log(level, msg);
		if (instance.console) instance.console->log(level, msg);
	}

	// Log a message with both loggers
	template<typename... ArgsT>
	static void log(LogLevel level,
	                const char* fmt,
	                ArgsT&&... args) {

		auto& instance = get();
		if (instance.file) instance.file->log(level, fmt, std::forward<ArgsT>(args)...);
		if (instance.console) instance.console->log(level, fmt, std::forward<ArgsT>(args)...);
	}

	// Set the level of both loggers
	static void setLevel(LogLevel level) {
		auto& instance = get();
		if (instance.file) instance.file->set_level(level);
		if (instance.console) instance.console->set_level(level);
	}


	//----------------------------------------------------------------------------------
	// Console Log (stdout)
	//----------------------------------------------------------------------------------

	// Log a message with the console logger
	template<typename T>
	static void logConsole(LogLevel level, const T& msg) {
		auto& instance = get();
		if (instance.console) instance.console->log(level, msg);
	}

	// Log a message with the console logger
	template<typename... ArgsT>
	static void logConsole(LogLevel level,
	                       const char* fmt,
	                       ArgsT&&... args) {

		auto& instance = get();
		if (instance.console) instance.console->log(level, fmt, std::forward<ArgsT>(args)...);
	}

	// Set the level of the console logger
	static void setConsoleLevel(LogLevel level) {
		auto& instance = get();
		if (instance.console) instance.console->set_level(level);
	}


	//----------------------------------------------------------------------------------
	// File Log
	//----------------------------------------------------------------------------------

	// Log a message with the file logger
	template<typename T>
	static void logFile(LogLevel level, const T& msg) {
		auto& instance = get();
		if (instance.file) instance.file->log(level, msg);
	}

	// Log a message with the file logger
	template<typename... ArgsT>
	static void logFile(LogLevel level,
	                    const char* fmt,
	                    ArgsT&&... args) {

		auto& instance = get();
		if (instance.file) instance.file->log(level, fmt, std::forward<ArgsT>(args)...);
	}

	// Set the level of the file logger
	static void setFileLevel(LogLevel level) {
		auto& instance = get();
		if (instance.file) instance.file->set_level(level);
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
