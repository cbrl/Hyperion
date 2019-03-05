#include "renderer.h"
#include "rendering_mgr.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"


Renderer::Renderer(DisplayConfig& display_config,
                   const RenderingConfig& rendering_config,
                   ID3D11Device& device,
                   ID3D11DeviceContext& device_context,
                   SwapChain& swap_chain,
                   ResourceMgr& resource_mgr)
	: device(device)
	, device_context(device_context)
    , display_config(display_config)
	, profiler(device, device_context)
	, engine_buffer(device) {

	profiler.registerTimestampID("ImGui");
	profiler.registerTimestampID("Text");
	profiler.registerTimestampID("Skybox");

	// Bind the engine buffer (stays bound for the engine's lifetime)
	engine_buffer.bind<Pipeline>(device_context, SLOT_CBUFFER_ENGINE);

	// Create state managers
	output_mgr       = std::make_unique<OutputMgr>(display_config, device, swap_chain);
	render_state_mgr = std::make_unique<RenderStateMgr>(device, device_context);

	// Create renderers
	light_pass           = std::make_unique<LightPass>(rendering_config, device, device_context, *render_state_mgr, resource_mgr);
	forward_pass         = std::make_unique<ForwardPass>(device, device_context, *render_state_mgr, resource_mgr);
	sky_pass             = std::make_unique<SkyPass>(device_context, *render_state_mgr, resource_mgr);
	bounding_volume_pass = std::make_unique<BoundingVolumePass>(device, device_context, *render_state_mgr, resource_mgr);
	text_pass            = std::make_unique<TextPass>(device_context);
}


void Renderer::render(Scene& scene, f32 delta_time) {

	//----------------------------------------------------------------------------------
	// Update the engine buffer
	//----------------------------------------------------------------------------------
	updateBuffers(delta_time);

	
	//----------------------------------------------------------------------------------
	// Begin a new profiler frame
	//----------------------------------------------------------------------------------
	profiler.beginFrame();


	profiler.beginTimestamp(GPUTimestamps::render_scene);
	
	//----------------------------------------------------------------------------------
	// Render the scene for each camera
	//----------------------------------------------------------------------------------
	scene.forEach<PerspectiveCamera>([&](const PerspectiveCamera& camera) {

		if (!camera.isActive()) return;

		// Bind the buffer and viewport
		camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);
		camera.bindViewport(device_context);

		// Render the scene
		renderCamera(scene, camera);
	});

	scene.forEach<OrthographicCamera>([&](const OrthographicCamera& camera) {

		if (!camera.isActive()) return;

		// Bind the buffer and viewport
		camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);
		camera.bindViewport(device_context);

		// Render the scene
		renderCamera(scene, camera);
	});


	//----------------------------------------------------------------------------------
	// Bind the final output state
	//----------------------------------------------------------------------------------
	output_mgr->bindEnd(device_context);


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("Text");
	text_pass->render(scene);
	profiler.endTimestamp("Text");

	profiler.endTimestamp(GPUTimestamps::render_scene);


	//----------------------------------------------------------------------------------
	// Render ImGui data
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("ImGui");
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	profiler.endTimestamp("ImGui");

	profiler.endFrame();
	profiler.update();
}


void Renderer::updateBuffers(f32 delta_time) {
	EngineBuffer buffer;

	buffer.resolution = display_config.getDisplayResolution();
	buffer.delta_time = delta_time;

	engine_buffer.updateData(device_context, buffer);
}


// This function template is only called from within this translation unit so it can be defined here as well
template<typename CameraT>
void Renderer::renderCamera(Scene& scene, const CameraT& camera) {

	// Camera variables
	const auto& settings  = camera.getSettings();
	const auto* transform = camera.getOwner()->getComponent<Transform>();
	assert(transform != nullptr);

	const auto  world_to_camera      = transform->getWorldToObjectMatrix();
	const auto  camera_to_projection = camera.getCameraToProjectionMatrix();
	const auto  world_to_projection  = world_to_camera * camera_to_projection;
	const auto* skybox               = camera.getSettings().getSkybox();


	//----------------------------------------------------------------------------------
	// Process the light buffers
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp(GPUTimestamps::shadow_maps);
	light_pass->render(scene, world_to_projection);
	profiler.endTimestamp(GPUTimestamps::shadow_maps);


	//----------------------------------------------------------------------------------
	// Bind the camera's viewport and the forward output state
	//----------------------------------------------------------------------------------
	camera.bindViewport(device_context);
	output_mgr->bindBeginForward(device_context);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("Skybox");
	sky_pass->render(skybox);
	profiler.endTimestamp("Skybox");


	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp(GPUTimestamps::forward_render);

	if (settings.getLightingMode() == LightingMode::Default) {
		using model_cref_vector = std::vector<std::reference_wrapper<const Model>>;

		// Sort models by shader type
		std::unordered_map<PixelShader*, model_cref_vector> sorted_models;
		scene.forEach<Model>([&](Model& model) {
			if (!model.isActive()) return;
			auto& mat = model.getMaterial();
			sorted_models[mat.shader.get()].push_back(std::cref(model));
		});

		// Render each list of models with its associated shader
		for (auto& [shader, model_vec] : sorted_models) {
			forward_pass->renderOpaque(model_vec, world_to_projection, skybox, shader);
		}
		for (auto& [shader, model_vec] : sorted_models) {
			forward_pass->renderTransparent(model_vec, world_to_projection, skybox, shader);
		}
	}
	else {
		switch (settings.getLightingMode()) {
			case LightingMode::BRDF:
				forward_pass->renderOpaque(scene, world_to_projection, skybox, settings.getBRDF());
				forward_pass->renderTransparent(scene, world_to_projection, skybox, settings.getBRDF());
				break;

			case LightingMode::FalseColor:
				forward_pass->renderFalseColor(scene, world_to_projection, settings.getFalseColorMode());
				break;

			default:
				break;
		}
	}

	profiler.endTimestamp(GPUTimestamps::forward_render);


	//----------------------------------------------------------------------------------
	// Render wireframes
	//----------------------------------------------------------------------------------
	if (settings.hasRenderOption(RenderOptions::Wireframe))
		forward_pass->renderWireframe(scene, world_to_projection, camera.getSettings().getWireframeColor());


	//----------------------------------------------------------------------------------
	// Render bounding volumes
	//----------------------------------------------------------------------------------
	if (settings.hasRenderOption(RenderOptions::BoundingVolume))
		bounding_volume_pass->render(scene, world_to_projection, camera.getSettings().getBoundingVolumeColor());


	//----------------------------------------------------------------------------------
	// Clear the bound forward state
	//----------------------------------------------------------------------------------
	output_mgr->bindEndForward(device_context);
}
