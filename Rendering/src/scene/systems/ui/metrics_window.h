#pragma once

#include "imgui.h"
#include "imgui_addons/metrics_gui/metrics_gui/metrics_gui.h"

class Engine;

class MetricsWindow final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	MetricsWindow(const Engine& engine);
	MetricsWindow(const MetricsWindow&) = default;
	MetricsWindow(MetricsWindow&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~MetricsWindow() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	MetricsWindow& operator=(const MetricsWindow&) = default;
	MetricsWindow& operator=(MetricsWindow&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void draw(Engine& engine);

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// FPS
	MetricsGuiPlot   frame_plot;
	MetricsGuiMetric frame_time{"Frame Time", "s", si_prefix};
	MetricsGuiMetric fps{"FPS", "", 0};

	// CPU Usage
	MetricsGuiPlot   cpu_plot;
	MetricsGuiMetric total_cpu{"Total Usage", "%", known_min_max};
	MetricsGuiMetric process_cpu{"Process Usage", "%", known_min_max};

	// RAM Usage
	MetricsGuiPlot   ram_plot;
	MetricsGuiMetric total_ram{"Total Usage", "B", si_prefix | known_min_max};
	MetricsGuiMetric process_ram{"Process Usage", "B", si_prefix | known_min_max};

	// GPU Time
	MetricsGuiPlot   gpu_plot;
	MetricsGuiMetric frame_gpu_time{"Frame Time (GPU)", "s", si_prefix};
	MetricsGuiMetric imgui_render{"ImGui Render", "s", si_prefix};

	// Scene Render GPU Time
	MetricsGuiPlot   scene_gpu_plot;
	MetricsGuiMetric scene_render{"Scene Render", "s", si_prefix};
	MetricsGuiMetric skybox_render{"Skybox Render", "s", si_prefix};
	MetricsGuiMetric shadow_maps{"Shadow Maps Render", "s", si_prefix};
	MetricsGuiMetric forward_render{"Forward Render", "s", si_prefix};
	MetricsGuiMetric text_render{"Text Render", "s", si_prefix};

	static constexpr auto si_prefix     = MetricsGuiMetric::USE_SI_UNIT_PREFIX;
	static constexpr auto known_min_max = MetricsGuiMetric::KNOWN_MIN_VALUE | MetricsGuiMetric::KNOWN_MAX_VALUE;
};