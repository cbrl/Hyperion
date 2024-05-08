module;

#include "imgui.h"
#include "imgui_addons/metrics_gui/metrics_gui/metrics_gui.h"

export module rendering:systems.user_interface.modules.metrics_window;

import :engine;
import :gpu_profiler;


export class MetricsWindow final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	MetricsWindow(const render::Engine& engine) {
		frame_plot.mShowInlineGraphs   = true;
		frame_plot.mInlinePlotRowCount = 3;
		frame_plot.mShowLegendAverage  = true;
		frame_plot.mShowLegendColor    = false;
		frame_plot.mShowLegendDesc     = false;
		frame_time.mSelected           = true;
		fps.mSelected                  = true;
		frame_plot.AddMetric(&fps);
		frame_plot.AddMetric(&frame_time);

		total_cpu.mKnownMinValue    = 0.0f;
		total_cpu.mKnownMaxValue    = 100.0f;
		process_cpu.mKnownMinValue  = 0.0f;
		process_cpu.mKnownMaxValue  = 100.0f;
		cpu_plot.mShowLegendMin     = false;
		cpu_plot.mShowLegendMax     = false;
		cpu_plot.mShowLegendAverage = true;
		cpu_plot.AddMetric(&total_cpu);
		cpu_plot.AddMetric(&process_cpu);

		total_ram.mKnownMinValue    = 0.0f;
		total_ram.mKnownMaxValue    = static_cast<float>(engine.getSysMon().memory().getPhysicalMemSize());
		process_ram.mKnownMinValue  = 0.0f;
		process_ram.mKnownMaxValue  = static_cast<float>(engine.getSysMon().memory().getPhysicalMemSize());
		ram_plot.mShowLegendAverage = true;
		ram_plot.mShowLegendMin     = false;
		ram_plot.AddMetric(&total_ram);
		ram_plot.AddMetric(&process_ram);

		gpu_plot.mShowLegendMin     = false;
		gpu_plot.mShowLegendMax     = false;
		gpu_plot.mShowLegendAverage = true;
		gpu_plot.AddMetric(&frame_gpu_time);
		gpu_plot.AddMetric(&scene_render);
		gpu_plot.AddMetric(&imgui_render);
		gpu_plot.LinkLegends(&scene_gpu_plot);

		scene_gpu_plot.mShowLegendMin     = false;
		scene_gpu_plot.mShowLegendMax     = false;
		scene_gpu_plot.mShowLegendAverage = true;
		scene_gpu_plot.AddMetric(&skybox_render);
		scene_gpu_plot.AddMetric(&shadow_maps);
		scene_gpu_plot.AddMetric(&forward_render);
		scene_gpu_plot.AddMetric(&gbuffer_render);
		scene_gpu_plot.AddMetric(&deferred_render);
		scene_gpu_plot.AddMetric(&text_render);
	}

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
	void draw(render::Engine& engine) {
		//----------------------------------------------------------------------------------
		// Update frame time
		//----------------------------------------------------------------------------------
		const auto& timer = engine.getTimer();
		const auto  dt    = static_cast<float>(timer.deltaTime().count());
		frame_time.AddNewValue(dt);
		fps.AddNewValue(1.0f / dt);
		frame_plot.UpdateAxes();


		//----------------------------------------------------------------------------------
		// Update CPU/RAM usage
		//----------------------------------------------------------------------------------
		const auto& sys_mon = engine.getSysMon();
		total_cpu.AddNewValue(static_cast<float>(sys_mon.cpu().getTotalCpuPercentage()));
		process_cpu.AddNewValue(static_cast<float>(sys_mon.cpu().getProcessCpuPercentage()));
		cpu_plot.UpdateAxes();

		total_ram.AddNewValue(static_cast<float>(sys_mon.memory().getTotalUsedPhysicalMem()));
		process_ram.AddNewValue(static_cast<float>(sys_mon.memory().getProcessUsedPhysicalMem()));
		ram_plot.UpdateAxes();


		//----------------------------------------------------------------------------------
		// Update GPU time
		//----------------------------------------------------------------------------------
		const auto& profiler = engine.getRenderingMgr().getProfiler();
		frame_gpu_time.AddNewValue(profiler.deltaTime("Frame"));
		imgui_render.AddNewValue(profiler.deltaTime("ImGui"));
		scene_render.AddNewValue(profiler.deltaTime("Render Scene"));
		gpu_plot.UpdateAxes();

		skybox_render.AddNewValue(profiler.deltaTime("Skybox"));
		shadow_maps.AddNewValue(profiler.deltaTime("Shadow Maps"));
		forward_render.AddNewValue(profiler.deltaTime("Forward"));
		gbuffer_render.AddNewValue(profiler.deltaTime("GBuffer"));
		deferred_render.AddNewValue(profiler.deltaTime("Deferred"));
		text_render.AddNewValue(profiler.deltaTime("Text"));
		scene_gpu_plot.UpdateAxes();


		//----------------------------------------------------------------------------------
		// Draw Window
		//----------------------------------------------------------------------------------
		ImGui::SetNextWindowSizeConstraints({500, 0}, {500, -1});
		if (ImGui::Begin("Metrics", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			if (ImGui::BeginTabBar("Metrics Tab Bar")) {

				if (ImGui::BeginTabItem("All")) {
					ImGui::Text("FPS");
					ImGui::Separator();
					frame_plot.DrawList();

					ImGui::Spacing();
					ImGui::Text("CPU");
					ImGui::Separator();
					cpu_plot.DrawHistory();

					ImGui::Spacing();
					ImGui::Text("RAM");
					ImGui::Separator();
					ram_plot.DrawHistory();

					ImGui::Spacing();
					ImGui::Text("GPU");
					ImGui::Separator();
					gpu_plot.DrawHistory();
					//scene_gpu_plot.DrawHistory();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("FPS")) {
					frame_plot.DrawList();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("CPU")) {
					cpu_plot.DrawHistory();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("RAM")) {
					ram_plot.DrawHistory();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("GPU")) {
					gpu_plot.DrawHistory();
					ImGui::Spacing();
					ImGui::Text("Scene Render Breakdown");
					scene_gpu_plot.DrawHistory();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}

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
	MetricsGuiMetric gbuffer_render{"GBuffer Render", "s", si_prefix};
	MetricsGuiMetric deferred_render{"Deferred Render", "s", si_prefix};
	MetricsGuiMetric text_render{"Text Render", "s", si_prefix};

	static constexpr auto si_prefix     = MetricsGuiMetric::USE_SI_UNIT_PREFIX;
	static constexpr auto known_min_max = MetricsGuiMetric::KNOWN_MIN_VALUE | MetricsGuiMetric::KNOWN_MAX_VALUE;
};