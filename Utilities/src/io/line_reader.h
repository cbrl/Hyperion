#pragma once

#include "io/io.h"
#include "log/log.h"
#include <regex>


class LineReader {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	LineReader() = default;
	LineReader(const LineReader& reader) = delete;
	LineReader(LineReader&& reader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~LineReader() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	LineReader& operator=(const LineReader& reader) = delete;
	LineReader& operator=(LineReader&& reader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Read File
	//----------------------------------------------------------------------------------
	void readFile(fs::path file, std::regex regex = std::regex(R"([^\s\t\r\n]+)")) {

		file_path    = std::move(file);
		search_regex = std::move(regex);

		if (!fs::exists(file_path)) {
			Logger::log(LogLevel::err, "File not found: {}", file_path.string());
			return;
		}

		std::ifstream file_stream(file_path.string());
		if (file_stream.fail()) {
			Logger::log(LogLevel::err, "Could not open file: {}", file_path.string());
			return;
		}

		std::string line;
		while (std::getline(file_stream, line)) {

			token_iterator = std::sregex_iterator(line.begin(), line.end(), search_regex);

			if (hasTokens()) {
				readLine();
			}
			readUnusedTokens();

			++line_number;
		}
	}


protected:
	//----------------------------------------------------------------------------------
	// Member Functions - Read Line
	//----------------------------------------------------------------------------------
	virtual void readLine() = 0;


	//----------------------------------------------------------------------------------
	// Member Functions - Tokens
	//----------------------------------------------------------------------------------
	template<typename T>
	T readToken();

	template<>
	std::string readToken();

	void readUnusedTokens() {
		for (; hasTokens(); ++token_iterator) {
			const auto token = readToken<std::string>();
			Logger::log(LogLevel::warn, "{}:{} - Unused token: {}", file_path.string(), line_number, token);
		}
	}

	bool hasTokens() const {
		static const std::sregex_iterator token_iterator_end;
		return token_iterator != token_iterator_end;
	}

	const std::sregex_iterator& getCurrentToken() const {
		return token_iterator;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::regex           search_regex;
	std::sregex_iterator token_iterator;
	
	fs::path file_path;
	size_t   line_number = 0;
};


#include "line_reader.tpp"