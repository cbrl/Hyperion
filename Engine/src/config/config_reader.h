#pragma once

#include "datatypes/datatypes.h"
#include "io/line_reader.h"
#include "resource/var_map.h"

#include "config/config_tokens.h"


class ConfigReader final : protected LineReader {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ConfigReader();
	ConfigReader(const ConfigReader& reader) = delete;
	ConfigReader(ConfigReader&& reader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ConfigReader() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ConfigReader& operator=(const ConfigReader& reader) = delete;
	ConfigReader& operator=(ConfigReader&& reader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void readConfig(const fs::path& file);

	void readLine() override;

	template<typename T>
	const T* getConfigVar(const std::string& key) {
		return config_vars.getVar<T>(key);
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	VarMap config_vars;
};
