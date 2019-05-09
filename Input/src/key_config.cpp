#include "key_config.h"

void KeyConfig::bindKey(const std::string& id, Keyboard::Keys key) {
	key_map[id] = key;
}

bool KeyConfig::bindIfNotBound(const std::string& id, Keyboard::Keys key) {
	if (not isKeyBound(id)) {
		bindKey(id, key);
		return true;
	}
	return false;
}

void KeyConfig::unbindKey(const std::string& id) {
	key_map[id] = Keyboard::None;
}

Keyboard::Keys KeyConfig::getKey(const std::string& id) noexcept {
	if (const auto it = key_map.find(id); it != key_map.end()) {
		return it->second;
	}
	const auto iter_bool = key_map.try_emplace(id, Keyboard::None);
	return iter_bool.first->second;
}

Keyboard::Keys KeyConfig::getKey(const std::string& id) const noexcept {
	if (const auto it = key_map.find(id); it != key_map.end()) {
		return it->second;
	}
	return Keyboard::None;
}

bool KeyConfig::isKeyBound(const std::string& id) const noexcept {
	return getKey(id) != Keyboard::None;
}

void to_json(json& j, const KeyConfig& cfg) {
	j = json{cfg.key_map};
}

void from_json(const json& j, KeyConfig& cfg) {
	for (const auto& [key, value] : j.items()) {
		cfg.bindKey(key, static_cast<Keyboard::Keys>(value.get<u32>()));
	}
}