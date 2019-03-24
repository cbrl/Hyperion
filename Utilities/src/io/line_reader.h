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

		std::ifstream file_stream(file_path);
		if (file_stream.fail()) {
			Logger::log(LogLevel::err, "LineReader could not open file: {}", file_path.string());
			return;
		}

		line_number = 1;
		std::string line;
		while (std::getline(file_stream, line)) {
			token_iterator = std::sregex_token_iterator(line.begin(), line.end(), search_regex);
			if (hasTokens()) {
				readLine();
			}
			++line_number;
		}
	}

	void readString(const std::string& input, std::regex regex = std::regex(R"([^\s\t\r\n]+)")) {

		file_path    = fs::path{};
		search_regex = std::move(regex);

		std::istringstream string_stream(input);
		if (string_stream.fail()) {
			Logger::log(LogLevel::err, "LineReader failed to create string stream");
			return;
		}

		line_number = 1;
		std::string line;
		while (std::getline(string_stream, line)) {
			token_iterator = std::sregex_token_iterator(line.begin(), line.end(), search_regex);
			if (hasTokens()) {
				readLine();
			}
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

	void readUnusedTokens() {
		while (hasTokens()) {
			const auto token = readToken<std::string>();
			Logger::log(LogLevel::warn, "{}:{} - Unused token: {}", getFilePath().string(), getLineNumber(), token);
		}
	}

	bool hasTokens() const noexcept {
		static const std::sregex_token_iterator token_iterator_end;
		return token_iterator != token_iterator_end;
	}

	std::string_view getCurrentToken() const noexcept {
		if (token_iterator->matched) {
			return { &*token_iterator->first,
			         static_cast<size_t>(token_iterator->second - token_iterator->first) };
		}
		return {};
	}


	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	const fs::path& getFilePath() const noexcept {
		return file_path;
	}

	size_t getLineNumber() const noexcept {
		return line_number;
	}


private:
	template<typename T>
	T readTokenImpl();

	template<>
	std::string_view readTokenImpl();

	template<>
	std::string readTokenImpl();


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::regex                 search_regex;
	std::sregex_token_iterator token_iterator;
	
	fs::path file_path;
	size_t   line_number = 1;
};


#include "line_reader.tpp"