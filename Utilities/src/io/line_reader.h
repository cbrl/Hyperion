#pragma once

#include "io/io.h"
#include "log/log.h"
#include <regex>

// TODO: Warn on unused tokens in line

class LineReader {
public:
	LineReader() = default;
	LineReader(const LineReader& reader) = delete;
	LineReader(LineReader&& reader) noexcept = default;

	virtual ~LineReader() = default;

	LineReader& operator=(const LineReader& reader) = delete;
	LineReader& operator=(LineReader&& reader) noexcept = default;

	void readFile(const fs::path file, std::regex regex) {

		file_path = std::move(file);
		search_regex = std::move(regex);

		if (!fs::exists(file)) {
			Logger::log(LogLevel::err, "File not found: {}", file.string());
			return;
		}

		std::ifstream file_stream(file.string());
		if (file_stream.fail()) {
			Logger::log(LogLevel::err, "Could not open file: {}", file.string());
			return;
		}

		line_number = 0;
		std::string line;
		while (std::getline(file_stream, line)) {
			token_iterator = std::sregex_iterator(line.begin(), line.end(), search_regex);
			if (hasTokens()) {
				readLine();
			}
			++line_number;
		}
	}


protected:
	virtual void readLine() = 0;

	template<typename T>
	T readToken();

	bool hasTokens() const {
		return token_iterator != token_iterator_end;
	}

	const std::sregex_iterator& getCurrentToken() const {
		return token_iterator;
	}


private:
	std::regex search_regex;
	std::sregex_iterator token_iterator;
	const std::sregex_iterator token_iterator_end;

	fs::path file_path;
	size_t line_number;
};


#include "line_reader.tpp"