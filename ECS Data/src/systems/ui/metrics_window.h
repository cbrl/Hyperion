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
	MetricsWindow(MetricsWindow&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~MetricsWindow() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	MetricsWindow& operator=(const MetricsWindow&) = default;
	MetricsWindow& operator=(MetricsWindow&&) = default;


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
	MetricsGuiMetric frame_time;
	MetricsGuiMetric fps;

	// CPU Usage
	MetricsGuiPlot   cpu_plot;
	MetricsGuiMetric total_cpu;
	MetricsGuiMetric process_cpu;

	// RAM Usage
	MetricsGuiPlot   ram_plot;
	MetricsGuiMetric total_ram;
	MetricsGuiMetric process_ram;

	// GPU Time
	MetricsGuiPlot   gpu_plot;
	MetricsGuiMetric frame_gpu_time;
	MetricsGuiMetric imgui_render;

	// Scene Render GPU Time
	MetricsGuiPlot   scene_gpu_plot;
	MetricsGuiMetric scene_render;
	MetricsGuiMetric skybox_render;
	MetricsGuiMetric shadow_maps;
	MetricsGuiMetric forward_render;
	MetricsGuiMetric text_render;

	static constexpr auto si_prefix     = MetricsGuiMetric::USE_SI_UNIT_PREFIX;
	static constexpr auto known_min_max = MetricsGuiMetric::KNOWN_MIN_VALUE | MetricsGuiMetric::KNOWN_MAX_VALUE;
};