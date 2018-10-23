#include "config_writer.h"
#include "config/config_tokens.h"
#include "io/file_writer.h"
#include "engine/engine.h"


namespace ConfigWriter {

	void write(const Engine& engine, const fs::path& path) {

		FileWriter writer(path);

		const auto& window        = engine.getWindow();
		const auto& rendering_mgr = engine.getRenderingMgr();
		const auto& display       = rendering_mgr.getDisplayConfig();
		const auto& rendering     = rendering_mgr.getRenderingConfig();

		const auto res = window.getClientSize();

		writer.writeLine(ConfigTokens::comment, "Display");
		writer.writeLine(ConfigTokens::width, ' ', res.x);
		writer.writeLine(ConfigTokens::height, ' ', res.y);
		writer.writeLine(ConfigTokens::refresh, ' ', display.getRoundedDisplayRefreshRate());
		writer.writeLine(ConfigTokens::vsync, ' ', display.isVsync());
		writer.writeLine(ConfigTokens::fullscreen, ' ', display.isFullscreen());

		writer.writeLine();

		writer.writeLine(ConfigTokens::comment, "Window");
		writer.writeLine(ConfigTokens::win_title, ' ', window.getWindowTitle());

		writer.writeLine();

		writer.writeLine(ConfigTokens::comment, "Engine");
		writer.writeLine(ConfigTokens::shadowmap_res, ' ', rendering.getShadowMapRes());
	}
}