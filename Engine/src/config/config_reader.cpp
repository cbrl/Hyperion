#include "config_reader.h"


ConfigReader::ConfigReader() {
	config_vars.addVar<u32>(ConfigTokens::width, 1200);
	config_vars.addVar<u32>(ConfigTokens::height, 900);
	config_vars.addVar<u32>(ConfigTokens::refresh, 60);
	config_vars.addVar(ConfigTokens::vsync, false);
	config_vars.addVar(ConfigTokens::fullscreen, false);
	config_vars.addVar<u32>(ConfigTokens::shadowmap_res, 512);
	config_vars.addVar<std::string>(ConfigTokens::win_title, "Engine");
}


void ConfigReader::readConfig(const fs::path& file) {
	readFile(file);
}


void ConfigReader::readLine() {

	const auto token = readToken<std::string>();

	if (token[0] == ConfigTokens::comment) {
		return;
	}

	else if (token == ConfigTokens::width) {
		const auto val = readToken<u32>();
		config_vars.addVar(ConfigTokens::width, val);
	}

	else if (token == ConfigTokens::height) {
		const auto val = readToken<u32>();
		config_vars.addVar(ConfigTokens::height, val);
	}

	else if (token == ConfigTokens::refresh) {
		const auto val = readToken<u32>();
		config_vars.addVar(ConfigTokens::refresh, val);
	}

	else if (token == ConfigTokens::vsync) {
		const auto val = readToken<bool>();
		config_vars.addVar(ConfigTokens::vsync, val);
	}

	else if (token == ConfigTokens::fullscreen) {
		const auto val = readToken<bool>();
		config_vars.addVar(ConfigTokens::fullscreen, val);
	}

	else if (token == ConfigTokens::win_title) {
		const auto val = readToken<std::string>();
		config_vars.addVar(ConfigTokens::win_title, val);
	}

	else if (token == ConfigTokens::shadowmap_res) {
		const auto val = readToken<u32>();
		config_vars.addVar(ConfigTokens::shadowmap_res, val);
	}

	readUnusedTokens();
}