#include "stdafx.h"
#include "renderer.h"

#include "util\engine_util.h"
#include "shader\hlsl.h"
#include "rendering\pipeline.h"
#include "imgui\imgui.h"


Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* device_context)
	: device_context(device_context)
{
	// Create renderers
	forward_renderer = make_unique<ForwardRenderer>(device, device_context);
	sky_renderer     = make_unique<SkyRenderer>(device, device_context);
}


void Renderer::Render(Scene& scene, RenderStateMgr& render_state_mgr) const {

	//----------------------------------------------------------------------------------
	// Update the camera buffer
	//----------------------------------------------------------------------------------

	// Update the camera 
	scene.GetCamera().UpdateBuffer(device_context.Get());

	// Bind the buffer
	scene.GetCamera().BindBuffer(device_context.Get(), SLOT_CBUFFER_CAMERA);
	

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

	scene.ForEach<Text>([](Text& text) {
		text.Render();
	});


	//----------------------------------------------------------------------------------
	// Render the ImGui UI
	//----------------------------------------------------------------------------------

	scene.DrawUI();
}
