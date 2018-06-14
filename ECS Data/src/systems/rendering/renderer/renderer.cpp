#include "renderer.h"
#include "engine/engine.h"


Renderer::Renderer(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context) {

	// Create renderers
	light_pass   = make_unique<LightPass>(device, device_context);
	forward_pass = make_unique<ForwardPass>(device, device_context);
	sky_pass     = make_unique<SkyPass>(device, device_context);
	text_pass    = make_unique<TextPass>(device_context);
	ui           = make_unique<UserInterface>();
}


void Renderer::update(const Engine& engine) {

	auto& ecs_engine = engine.getECS();
	auto& scene      = engine.getScene();


	ecs_engine.forEachActive<PerspectiveCamera>([&](const PerspectiveCamera& camera) {
		// Bind the buffer
		camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);

		// Render the scene
		renderCamera(engine, camera);
	});

	ecs_engine.forEachActive<OrthographicCamera>([&](const OrthographicCamera& camera) {
		// Bind the buffer
		camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);

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

