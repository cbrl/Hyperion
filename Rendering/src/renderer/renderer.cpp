#include "renderer.h"
#include "rendering_mgr.h"


Renderer::Renderer(DisplayConfig& display_config, 
                   ID3D11Device& device,
                   ID3D11DeviceContext& device_context,
                   SwapChain& swap_chain,
                   ResourceMgr& resource_mgr)
	: device(device)
	, device_context(device_context) {

	output_mgr       = std::make_unique<OutputMgr>(display_config, device, swap_chain);
	render_state_mgr = std::make_unique<RenderStateMgr>(device, device_context);

	// Create renderers
	light_pass           = std::make_unique<LightPass>(device, device_context, *render_state_mgr, resource_mgr);
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


	// Bind the final output state
	output_mgr->bindEnd(device_context);


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------

	text_pass->render(scene);
}

