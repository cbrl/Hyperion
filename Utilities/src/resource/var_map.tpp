#include "log/log.h"

template<typename T>
[[nodiscard]]
bool VarMap::contains(const std::string& key) const {
	if (const auto it = variables.find(key); it != variables.end()) {
		return std::get_if<T>(&(it->second)) != nullptr;
	}

	return false;
}

template<typename T>
void VarMap::addVar(const std::string& key, T value) {
	variables.insert_or_assign(key, std::move(value));
}

template<typename T>
void VarMap::setVar(const std::string& key, T value) {
	if (const auto it = variables.find(key); it != variables.end()) {
		it->second = std::move(value);
	}
	else {
		Logger::log(LogLevel::warn, "Variable not found: {}", key);
	}
}

template<typename T>
[[nodiscard]]
const T* VarMap::getVar(const std::string& key) const {
	if (const auto it = variables.find(key); it != variables.end()) {
		//return std::get_if<T>(it->second);
		return &std::get<T>(it->second);
	}

	Logger::log(LogLevel::warn, "Variable not found: {}", key);
	return nullptr;
}