#pragma once

#include "datatypes/datatypes.h"
#include "io/line_reader.h"
#include "resource/var_map.h"

#include "config/config_tokens.h"


class ConfigReader final : protected LineReader {
public:
	ConfigReader();
	ConfigReader(const ConfigReader& reader) = delete;
	ConfigReader(ConfigReader&& reader) noexcept = default;

	~ConfigReader() = default;

	ConfigReader& operator=(const ConfigReader& reader) = delete;
	ConfigReader& operator=(ConfigReader&& reader) noexcept = default;

	void readConfig(const fs::path& file);

	void readLine() override;

	template<typename T>
	const T* getConfigVar(const std::string& key) {
		return config_vars.getVar<T>(key);
	}


private:
	VarMap config_vars;
};
