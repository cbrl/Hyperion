#include "stdafx.h"
#include "renderer.h"

#include "imgui\imgui.h"
#include "engine\engine.h"


Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* device_context)
	: device_context(device_context)
{
	// Create renderers
	text_renderer    = make_unique<TextRenderer>(device_context);
	forward_renderer = make_unique<ForwardRenderer>(device, device_context);
	sky_renderer     = make_unique<SkyRenderer>(device, device_context);
}


void Renderer::Render(Engine& engine, RenderStateMgr& render_state_mgr) const {

	// Get the scene
	Scene& scene = engine.GetScene();

	//----------------------------------------------------------------------------------
	// Update the camera buffer
	//----------------------------------------------------------------------------------

	// Update the camera 
	auto* transform = ECS::Get()->GetComponent<CameraTransform>(scene.GetCamera());
	ECS::Get()->GetComponent<PerspectiveCamera>(scene.GetCamera())->UpdateBuffer(device_context.Get(), transform->GetTranslationMatrix(), transform->GetPosition());

	// Bind the buffer
	ECS::Get()->GetComponent<PerspectiveCamera>(scene.GetCamera())->BindBuffer(device_context.Get(), SLOT_CBUFFER_CAMERA);
	

	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	
	forward_renderer->Render(scene, render_state_mgr);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------

	sky_renderer->Render(scene, render_state_mgr);


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------

	text_renderer->Render(scene);


	//----------------------------------------------------------------------------------
	// Render the ImGui UI
	//----------------------------------------------------------------------------------

	//scene.DrawUI(engine);
}
