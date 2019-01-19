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
	, device_context(device_context) {

	output_mgr       = std::make_unique<OutputMgr>(display_config, device, swap_chain);
	render_state_mgr = std::make_unique<RenderStateMgr>(device, device_context);

	// Create renderers
	light_pass           = std::make_unique<LightPass>(rendering_config, device, device_context, *render_state_mgr, resource_mgr);
	forward_pass         = std::make_unique<ForwardPass>(device, device_context, *render_state_mgr, resource_mgr);
	sky_pass             = std::make_unique<SkyPass>(device_context, *render_state_mgr, resource_mgr);
	bounding_volume_pass = std::make_unique<BoundingVolumePass>(device, device_context, *render_state_mgr, resource_mgr);
	text_pass            = std::make_unique<TextPass>(device_context);
}


void Renderer::render(Scene& scene) {

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
	text_pass->render(scene);


	//----------------------------------------------------------------------------------
	// Render ImGui data
	//----------------------------------------------------------------------------------
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
	light_pass->render(scene, world_to_projection);


	//----------------------------------------------------------------------------------
	// Bind the camera's viewport and the forward output state
	//----------------------------------------------------------------------------------
	camera.bindViewport(device_context);
	output_mgr->bindBeginForward(device_context);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------
	sky_pass->render(skybox);


	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	switch (settings.getLightingMode()) {
		case LightingMode::Default:
			forward_pass->renderOpaque(scene, world_to_projection, skybox, settings.getBRDF());
			forward_pass->renderTransparent(scene, world_to_projection, skybox, settings.getBRDF());
			break;

		case LightingMode::FalseColorFullbright:
			forward_pass->renderFalseColor(scene, world_to_projection, FalseColor::Fullbright);
			break;

		case LightingMode::FalseColorNormal:
			forward_pass->renderFalseColor(scene, world_to_projection, FalseColor::Normal);
			break;

		case LightingMode::FalseColorDepth:
			forward_pass->renderFalseColor(scene, world_to_projection, FalseColor::Depth);
			break;

		default:
			break;
	}


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
