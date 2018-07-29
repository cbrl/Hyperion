#include "renderer.h"
#include "rendering_mgr.h"


Renderer::Renderer(ID3D11Device& device,
                   ID3D11DeviceContext& device_context,
                   RenderStateMgr& render_state_mgr,
                   ResourceMgr& resource_mgr)
	: device(device)
	, device_context(device_context) {

	// Create renderers
	light_pass           = std::make_unique<LightPass>(device, device_context, render_state_mgr, resource_mgr);
	forward_pass         = std::make_unique<ForwardPass>(device, device_context, render_state_mgr, resource_mgr);
	sky_pass             = std::make_unique<SkyPass>(device_context, render_state_mgr, resource_mgr);
	bounding_volume_pass = std::make_unique<BoundingVolumePass>(device, device_context, render_state_mgr, resource_mgr);
	text_pass            = std::make_unique<TextPass>(device_context);
	ui                   = std::make_unique<UserInterface>(device, resource_mgr);
}


void Renderer::render(const RenderingMgr& rendering_mgr, Scene& scene) {

	auto& ecs_engine = scene.getECS();

	ecs_engine.forEach<PerspectiveCamera>([&](const PerspectiveCamera& camera) {

		if (!camera.isActive()) return;

		// Bind the buffer and viewport
		camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);
		camera.bindViewport(device_context);

		// Render the scene
		renderCamera(rendering_mgr, scene, camera);
	});

	ecs_engine.forEach<OrthographicCamera>([&](const OrthographicCamera& camera) {

		if (!camera.isActive()) return;

		// Bind the buffer and viewport
		camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);
		camera.bindViewport(device_context);

		// Render the scene
		renderCamera(rendering_mgr, scene, camera);
	});


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------

	text_pass->render(scene);


	//----------------------------------------------------------------------------------
	// Render the ImGui UI
	//----------------------------------------------------------------------------------

	ui->draw(scene);
}

