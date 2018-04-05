#include "stdafx.h"
#include "forward_render.h"

#include "util\math\math.h"
#include "rendering\pipeline.h"
#include "shader\hlsl.h"


ForwardRenderer::ForwardRenderer(ID3D11Device* device, ID3D11DeviceContext* device_context)
	: device(device)
	, device_context(device_context)
	, light_buffer(device)
	, directional_light_buffer(device, 16)
	, point_light_buffer(device, 16)
	, spot_light_buffer(device, 16)
{

	// Create the vertex shader
	vertex_shader = make_unique<VertexShader>(device, L"shaders/forward/forward_vs.hlsl",
											  VertexPositionNormalTexture::InputElements,
											  VertexPositionNormalTexture::InputElementCount);

	// Create the pixel shader
	pixel_shader = make_unique<PixelShader>(device, L"shaders/forward/forward.hlsl");
}


void ForwardRenderer::Render(Scene& scene, RenderStateMgr& render_state_mgr) {

	//----------------------------------------------------------------------------------
	// Bind buffers
	//----------------------------------------------------------------------------------

	light_buffer.Bind<Pipeline::PS>(device_context.Get(), SLOT_CBUFFER_LIGHT);


	//----------------------------------------------------------------------------------
	// Bind SRVs
	//----------------------------------------------------------------------------------

	directional_light_buffer.Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_DIRECTIONAL_LIGHTS);
	point_light_buffer.Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_POINT_LIGHTS);
	spot_light_buffer.Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_SPOT_LIGHTS);


	//----------------------------------------------------------------------------------
	// Bind shaders
	//----------------------------------------------------------------------------------

	pixel_shader->Bind(device_context.Get());
	vertex_shader->Bind(device_context.Get());


	//----------------------------------------------------------------------------------
	// Bind the render states
	//----------------------------------------------------------------------------------

	render_state_mgr.BindDepthDefault(device_context.Get());
	render_state_mgr.BindCullCounterClockwise(device_context.Get());


	//----------------------------------------------------------------------------------
	// Get the matrices
	//----------------------------------------------------------------------------------

	const XMMATRIX view       = scene.GetCamera().GetViewMatrix();
	const XMMATRIX projection = scene.GetCamera().GetProjMatrix();


	//----------------------------------------------------------------------------------
	// Get the frustum
	//----------------------------------------------------------------------------------

	const auto& frustum = scene.GetCamera().GetFrustum();


	//----------------------------------------------------------------------------------
	// Update buffers
	//----------------------------------------------------------------------------------

	// Update light buffer
	LightBuffer light_data;
	light_data.directional_light_count = static_cast<u32>(scene.GetDirectionalLights().size());
	light_data.point_light_count       = static_cast<u32>(scene.GetPointLights().size());
	light_data.spot_light_count        = static_cast<u32>(scene.GetSpotLights().size());

	light_data.fog_color = scene.GetFog().color;
	light_data.fog_start = scene.GetFog().start;
	light_data.fog_range = scene.GetFog().range;

	light_buffer.UpdateData(device_context.Get(), light_data);


	// Update light data buffers
	directional_light_buffer.UpdateData(device.Get(), device_context.Get(), scene.GetDirectionalLights());
	point_light_buffer.UpdateData(device.Get(),       device_context.Get(), scene.GetPointLights());
	spot_light_buffer.UpdateData(device.Get(),        device_context.Get(), scene.GetSpotLights());
	

	//----------------------------------------------------------------------------------
	// Render models
	//----------------------------------------------------------------------------------

	scene.ForEach<Model>([&](Model& model) {

		// Cull the model if it isn't on screen
		if (!frustum.Contains(model.GetAABB())) return;


		// Update the model's object matrix, buffer, and bounding volumes
		model.Update(device_context.Get(), view, projection);


		// Bind the model's mesh
		model.Bind(device_context.Get());


		// Render each model part individually
		model.ForEachChild([&](ChildModel& child) {

			if (!frustum.Contains(child.GetAABB())) return;

			// Bind the child model's buffer
			child.BindBuffer<Pipeline::VS>(device_context.Get(), SLOT_CBUFFER_MODEL);
			child.BindBuffer<Pipeline::PS>(device_context.Get(), SLOT_CBUFFER_MODEL);

			// Get the child model's material
			const auto& mat = child.GetMaterial();

			// Bind the SRVs
			if (mat.map_Kd)   mat.map_Kd->Bind<Pipeline::PS>(device_context.Get(),   SLOT_SRV_DIFFUSE);
			if (mat.map_Ka)   mat.map_Ka->Bind<Pipeline::PS>(device_context.Get(),   SLOT_SRV_AMBIENT);
			if (mat.map_Ks)   mat.map_Ks->Bind<Pipeline::PS>(device_context.Get(),   SLOT_SRV_SPECULAR);
			if (mat.map_Ns)   mat.map_Ns->Bind<Pipeline::PS>(device_context.Get(),   SLOT_SRV_SPEC_HIGHLIGHT);
			if (mat.map_d)    mat.map_d->Bind<Pipeline::PS>(device_context.Get(),    SLOT_SRV_ALPHA);
			if (mat.map_bump) mat.map_bump->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_NORMAL);


			model.Draw(device_context.Get(), child.GetIndexCount(), child.GetIndexStart());
		});
	});
}
