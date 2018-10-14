#include "string/string.h"
#include "exception/exception.h"


template<typename T>
T LineReader::readToken() {
	if (!hasTokens()) {
		Logger::log(LogLevel::err, "{}:{} - No token value to read", file_path.string(), line_number);
		throw std::runtime_error("No token value to read");
	}

	const std::string str = token_iterator->str();

	const auto out = StrTo<T>(str);
	if (!out.has_value()) {
		Logger::log(LogLevel::err, "Invalid value: {}", str);
		throw std::runtime_error("Invalid value: " + str);
	}

	++token_iterator;
	return out.value();
}

template<>
inline std::string LineReader::readToken() {
	if (!hasTokens()) {
		Logger::log(LogLevel::err, "{}:{} - No token value to read", file_path.string(), line_number);
		throw std::runtime_error("No token value to read");
	}

	const std::string str = token_iterator->str();
	++token_iterator;
	return str;
}