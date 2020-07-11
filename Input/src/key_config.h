#pragma once

#include "json/nlohmann_json.h"
#include "directx/directxtk.h"


class KeyConfig final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	KeyConfig() = default;
	KeyConfig(const KeyConfig&) = default;
	KeyConfig(KeyConfig&&) = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~KeyConfig() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	KeyConfig& operator=(const KeyConfig&) = default;
	KeyConfig& operator=(KeyConfig&&) = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void bindKey(const std::string& id, Keyboard::Keys key);

	// Bind a key if it's not bound yet. Returns true if the function binds a key.
	bool tryBindKey(const std::string& id, Keyboard::Keys key);

	void unbindKey(const std::string& id);

	// Get the key bound to the identifier. Creates a new entry if one isn't found.
	[[nodiscard]]
	Keyboard::Keys getKey(const std::string& id) noexcept;

	// Get the key bound to the identifier
	[[nodiscard]]
	Keyboard::Keys getKey(const std::string& id) const noexcept;

	[[nodiscard]]
	bool isKeyBound(const std::string& id) const noexcept;

	friend void to_json(nl::json& j, const KeyConfig& cfg);

	friend void from_json(const nl::json& j, KeyConfig& cfg);

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::map<std::string, Keyboard::Keys> key_map;
};