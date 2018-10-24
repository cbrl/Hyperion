#include "string/string.h"
#include "exception/exception.h"


template<typename T>
T LineReader::readToken() {
	if (!hasTokens()) {
		Logger::log(LogLevel::err, "{}:{} - No token value to read", file_path.string(), line_number);
		throw std::runtime_error("No token value to read");
	}

	const auto out = readTokenImpl<T>();

	++token_iterator;
	return out;
}


template<typename T>
T LineReader::readTokenImpl() {

	const std::string_view str = getCurrentToken();

	const auto out = StrTo<T>(str);
	if (!out.has_value()) {
		Logger::log(LogLevel::err, "Invalid value: {}", str);
		throw std::runtime_error("Invalid value: " + std::string(str));
	}
	
	return out.value();
}


template<>
inline std::string_view LineReader::readTokenImpl() {
	return getCurrentToken();
}


template<>
inline std::string LineReader::readTokenImpl() {
	return token_iterator->str();
}