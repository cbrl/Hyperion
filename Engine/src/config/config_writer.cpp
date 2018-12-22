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
		
	    const auto title      = WstrToStr(window.getWindowTitle());
		const auto res        = window.getClientSize();
	    const auto refresh    = display.getRoundedDisplayRefreshRate();
	    const auto vsync      = display.isVsync();
	    const auto fullscreen = display.isFullscreen();
	    const auto shadowmap  = rendering.getShadowMapRes();

		writer.writeLine(ConfigTokens::comment, "Display");
		writer.writeLine(ConfigTokens::width, ' ', res.x);
		writer.writeLine(ConfigTokens::height, ' ', res.y);
		writer.writeLine(ConfigTokens::refresh, ' ', refresh);
		writer.writeLine(ConfigTokens::vsync, ' ', vsync);
		writer.writeLine(ConfigTokens::fullscreen, ' ', fullscreen);

		writer.writeLine();

		writer.writeLine(ConfigTokens::comment, "Window");
		writer.writeLine(ConfigTokens::win_title, ' ', title.empty() ? "Engine" : title);

		writer.writeLine();

		writer.writeLine(ConfigTokens::comment, "Engine");
		writer.writeLine(ConfigTokens::shadowmap_res, ' ', shadowmap);
	}
}