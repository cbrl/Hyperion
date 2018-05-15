#include "stdafx.h"
#include "renderer.h"
#include "engine\engine.h"


Renderer::Renderer(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context)
{
	// Create renderers
	forward_pass = make_unique<ForwardPass>(device, device_context);
	sky_pass     = make_unique<SkyPass>(device, device_context);
	text_pass    = make_unique<TextPass>(device_context);
}


void Renderer::Update(const Engine& engine) {

	auto& ecs_engine             = engine.GetECS();
	const auto& render_state_mgr = engine.GetRenderingMgr().GetRenderStateMgr();
	auto& scene                  = engine.GetScene();

	//----------------------------------------------------------------------------------
	// Update the camera buffer
	//----------------------------------------------------------------------------------

	// Update the camera 
	auto* transform = ecs_engine.GetComponent<CameraTransform>(scene.GetCamera());
	ecs_engine.GetComponent<PerspectiveCamera>(scene.GetCamera())->UpdateBuffer(device_context, transform->GetPositionMatrix(), transform->GetPosition());

	// Bind the buffer
	ecs_engine.GetComponent<PerspectiveCamera>(scene.GetCamera())->BindBuffer(device_context, SLOT_CBUFFER_CAMERA);
	

	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	
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
