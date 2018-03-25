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
	scene.camera->UpdateBuffer(device_context.Get());

	// Bind the buffer
	scene.camera->BindBuffer(device_context.Get(), SLOT_CBUFFER_CAMERA);



	// Edit scene lights
	//for (u32 i = 0; i < scene.directional_lights.size(); ++i) {
	//	string name = "DirLight Color " + to_string(i);
	//	ImGui::ColorEdit4(name.c_str(), (float*)&scene.directional_lights[i].diffuse_color);
	//	name = "DirLight Direction " + to_string(i);
	//	ImGui::DragFloat3(name.c_str(), (float*)&scene.directional_lights[i].direction, 0.05f, -1.0f, 1.0f);
	//}
	//ImGui::ColorEdit4("Point Light Dif", (float*)&scene.point_lights[0].diffuse_color);
	//ImGui::ColorEdit4("Point Light Amb", (float*)&scene.point_lights[0].ambient_color);
	//ImGui::DragFloat3("Point Light Att", (float*)&scene.point_lights[0].attenuation, 0.01f, 0.0f, 1.0f);
	//ImGui::DragFloat3("Point Light Pos", (float*)&scene.point_lights[0].position, 0.1f, -50.0f, 50.0f);
	//ImGui::DragFloat("Point Light Range", (float*)&scene.point_lights[0].range, 0.05f, 0.0f, 200.0f);

	// Edit scene fog
	//ImGui::ColorEdit4("Fog Color", (float*)&scene.fog.color);
	//ImGui::DragFloat("Fog Start", (float*)&scene.fog.start, 1.0f, 0.0f, 200.0f);
	//ImGui::DragFloat("Fog Range", (float*)&scene.fog.range, 1.0f, 0.0f, 200.0f);

	

	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------

	sky_renderer->Render(scene, render_state_mgr);


	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	
	forward_renderer->Render(scene, render_state_mgr);


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------

	scene.ForEach<Text>([](Text& text) {
		text.Render();
	});
}
