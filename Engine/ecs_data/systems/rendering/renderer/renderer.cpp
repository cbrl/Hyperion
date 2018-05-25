#include "stdafx.h"
#include "renderer.h"
#include "engine\engine.h"


Renderer::Renderer(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context)
{
	// Create renderers
	light_pass   = make_unique<LightPass>(device, device_context);
	forward_pass = make_unique<ForwardPass>(device, device_context);
	sky_pass     = make_unique<SkyPass>(device, device_context);
	text_pass    = make_unique<TextPass>(device_context);
}


void Renderer::Update(const Engine& engine) {

	auto& ecs_engine             = engine.GetECS();
	const auto& rendering_mgr    = engine.GetRenderingMgr();
	const auto& render_state_mgr = rendering_mgr.GetRenderStateMgr();
	auto& scene                  = engine.GetScene();

	//----------------------------------------------------------------------------------
	// Update the camera buffer
	//----------------------------------------------------------------------------------

	// Update the camera 
	const auto transform = ecs_engine.GetComponent<CameraTransform>(scene.GetCamera());
	const auto camera    = ecs_engine.GetComponent<PerspectiveCamera>(scene.GetCamera());
	ecs_engine.GetComponent<PerspectiveCamera>(scene.GetCamera())->UpdateBuffer(device_context, transform->GetObjectToWorldMatrix(), transform->GetWorldOrigin());

	const auto world_to_camera = transform->GetWorldToObjectMatrix();
	const auto camera_to_projection = camera->GetProjectionMatrix();
	const auto world_to_projection = world_to_camera * camera_to_projection;

	// Bind the buffer
	ecs_engine.GetComponent<PerspectiveCamera>(scene.GetCamera())->BindBuffer(device_context, SLOT_CBUFFER_CAMERA);


	//----------------------------------------------------------------------------------
	// Process the light buffers
	//----------------------------------------------------------------------------------

	light_pass->Render(engine, world_to_projection);
	

	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	
	camera->BindViewport(device_context);
	rendering_mgr.BindDefaultRenderTarget();
	forward_pass->Render(engine);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------

	sky_pass->Render(engine);


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------

	text_pass->Render(scene);


	//----------------------------------------------------------------------------------
	// Render the ImGui UI
	//----------------------------------------------------------------------------------

	//scene.DrawUI(engine);
}
