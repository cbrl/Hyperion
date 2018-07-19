#include "renderer.h"
#include "engine/engine.h"


Renderer::Renderer(ID3D11Device& device,
                   ID3D11DeviceContext& device_context,
                   RenderStateMgr& render_state_mgr,
                   ResourceMgr& resource_mgr)
	: device(device)
	, device_context(device_context) {

	// Create renderers
	light_pass           = make_unique<LightPass>(device, device_context, render_state_mgr, resource_mgr);
	forward_pass         = make_unique<ForwardPass>(device, device_context, render_state_mgr, resource_mgr);
	sky_pass             = make_unique<SkyPass>(device, device_context, render_state_mgr, resource_mgr);
	bounding_volume_pass = make_unique<BoundingVolumePass>(device, device_context, render_state_mgr, resource_mgr);
	text_pass            = make_unique<TextPass>(device_context);
	ui                   = make_unique<UserInterface>();
}


void Renderer::render(const Engine& engine) {

	auto& ecs_engine = engine.getScene().getECS();
	auto& scene      = engine.getScene();


	ecs_engine.forEach<PerspectiveCamera>([&](const PerspectiveCamera& camera) {

		if (!camera.isActive()) return;

		// Bind the buffer and viewport
		camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);
		camera.bindViewport(device_context);

		// Render the scene
		renderCamera(engine, camera);
	});

	ecs_engine.forEach<OrthographicCamera>([&](const OrthographicCamera& camera) {

		if (!camera.isActive()) return;

		// Bind the buffer and viewport
		camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);
		camera.bindViewport(device_context);

		// Render the scene
		renderCamera(engine, camera);
	});


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------

	text_pass->render(scene);


	//----------------------------------------------------------------------------------
	// Render the ImGui UI
	//----------------------------------------------------------------------------------

	ui->draw(engine);
}

