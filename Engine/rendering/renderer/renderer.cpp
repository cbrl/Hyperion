#include "stdafx.h"
#include "renderer.h"
#include "imgui\imgui.h"
#include "engine\engine.h"


Renderer::Renderer(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device_context(device_context)
{
	// Create renderers
	text_renderer    = make_unique<TextRenderer>(device_context);
	forward_renderer = make_unique<ForwardRenderer>(device, device_context);
	sky_renderer     = make_unique<SkyRenderer>(device, device_context);
}


void Renderer::Render(const Engine& engine) const {

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
	
	forward_renderer->Render(engine);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------

	sky_renderer->Render(engine);


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------

	text_renderer->Render(scene);


	//----------------------------------------------------------------------------------
	// Render the ImGui UI
	//----------------------------------------------------------------------------------

	//scene.DrawUI(engine);
}
