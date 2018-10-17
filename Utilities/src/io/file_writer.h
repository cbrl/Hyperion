#pragma once

#include "io/io.h"
#include "log/log.h"


class FileWriter {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	FileWriter(const fs::path& file) {
		outfile.open(file);
		if (outfile.fail()) {
			Logger::log(LogLevel::err, "Could not open file for writing: {}", file.string());
		}
	}

	FileWriter(const FileWriter& writer) = delete;
	FileWriter(FileWriter&& writer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~FileWriter() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	FileWriter& operator=(const FileWriter& writer) = delete;
	FileWriter& operator=(FileWriter&& writer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	template<typename... ArgsT>
	void write(ArgsT&&... args) {
		if (outfile.is_open() && !outfile.fail()) {
			(outfile << ... << args);
		}
	}

	template<typename... ArgsT>
	void writeLine(ArgsT&&... args) {
		if (outfile.is_open() && !outfile.fail()) {
			(outfile << ... << args) << "\n";
		}
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::ofstream outfile;
};