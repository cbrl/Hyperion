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


void ForwardRenderer::Render(Scene& scene, const RenderStateMgr& render_state_mgr) {

	// Bind buffers
	light_buffer.Bind<Pipeline::PS>(device_context.Get(), SLOT_CBUFFER_LIGHT);


	//----------------------------------------------------------------------------------
	// Bind SRVs
	//----------------------------------------------------------------------------------
	directional_light_buffer.Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_DIRECTIONAL_LIGHTS);
	point_light_buffer.Bind<Pipeline::PS>(device_context.Get(),       SLOT_SRV_POINT_LIGHTS);
	spot_light_buffer.Bind<Pipeline::PS>(device_context.Get(),        SLOT_SRV_SPOT_LIGHTS);

	auto texture = scene.GetCamera().GetSkybox().GetTexture();
	if (texture) texture->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_SKYBOX);



	// Bind shaders
	pixel_shader->Bind(device_context.Get());
	vertex_shader->Bind(device_context.Get());


	// Bind the render states
	BindRenderStates(scene, render_state_mgr);


	// Update the light buffers
	UpdateLightBuffers(scene);


	// Render the models
	RenderModels(scene);
}


void ForwardRenderer::BindRenderStates(Scene& scene, const RenderStateMgr& render_state_mgr) const {

	switch (scene.GetRenderStates().blend_state) {
		case BlendStates::Default:
		case BlendStates::Opaque:
			render_state_mgr.BindOpaque(device_context.Get());
			break;
		case BlendStates::AlphaBlend:
			render_state_mgr.BindAlphaBlend(device_context.Get());
			break;
		case BlendStates::Additive:
			render_state_mgr.BindAdditive(device_context.Get());
			break;
		case BlendStates::NonPremultiplied:
			render_state_mgr.BindNonPremultiplied(device_context.Get());
			break;
	}

	switch (scene.GetRenderStates().depth_state) {
		case DepthStates::Default:
		case DepthStates::DepthDefault:
			render_state_mgr.BindDepthDefault(device_context.Get());
			break;
		case DepthStates::DepthNone:
			render_state_mgr.BindDepthNone(device_context.Get());
			break;
		case DepthStates::DepthRead:
			render_state_mgr.BindDepthRead(device_context.Get());
			break;
	}

	switch (scene.GetRenderStates().raster_state) {
		case RasterStates::Default:
		case RasterStates::CullCounterClockwise:
			render_state_mgr.BindCullCounterClockwise(device_context.Get());
			break;
		case RasterStates::CullClockwise:
			render_state_mgr.BindCullClockwise(device_context.Get());
			break;
		case RasterStates::CullNone:
			render_state_mgr.BindCullNone(device_context.Get());
			break;
		case RasterStates::Wireframe:
			render_state_mgr.BindWireframe(device_context.Get());
	}
}


void ForwardRenderer::UpdateLightBuffers(Scene& scene) {

	// Update light buffer
	LightBuffer light_data;
	light_data.directional_light_count = static_cast<u32>(scene.GetDirectionalLights().size());
	light_data.point_light_count = static_cast<u32>(scene.GetPointLights().size());
	light_data.spot_light_count = static_cast<u32>(scene.GetSpotLights().size());

	light_data.fog_color = scene.GetFog().color;
	light_data.fog_start = scene.GetFog().start;
	light_data.fog_range = scene.GetFog().range;

	light_buffer.UpdateData(device_context.Get(), light_data);


	// Update light data buffers
	directional_light_buffer.UpdateData(device.Get(), device_context.Get(), scene.GetDirectionalLights());
	point_light_buffer.UpdateData(device.Get(), device_context.Get(), scene.GetPointLights());
	spot_light_buffer.UpdateData(device.Get(), device_context.Get(), scene.GetSpotLights());
}


void ForwardRenderer::RenderModels(Scene & scene) const {

	// Get the scene's camera and its frustum
	auto& camera        = scene.GetCamera();
	const auto& frustum = camera.GetFrustum();


	// Get the matrices
	const XMMATRIX view = camera.GetViewMatrix();
	const XMMATRIX projection = camera.GetProjMatrix();


	scene.ForEach<Model>([&](Model& model) {

		// Cull the model if it isn't on screen
		if (!frustum.Contains(model.GetAABB())) return;


		// Update the model's object matrix, buffer, and bounding volumes
		model.Update(device_context.Get(), view, projection);


		// Bind the model's mesh
		model.Bind(device_context.Get());


		// Render each model part individually
		model.ForEachChild([&](ModelChild& child) {

			if (!frustum.Contains(child.GetAABB())) return;

			// Bind the child model's buffer
			child.BindBuffer<Pipeline::VS>(device_context.Get(), SLOT_CBUFFER_MODEL);
			child.BindBuffer<Pipeline::PS>(device_context.Get(), SLOT_CBUFFER_MODEL);

			// Get the child model's material
			const auto& mat = child.GetMaterial();

			// Bind the SRVs
			if (mat.map_diffuse)        mat.map_diffuse->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_DIFFUSE);
			if (mat.map_ambient)        mat.map_ambient->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_AMBIENT);
			if (mat.map_specular)       mat.map_specular->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_SPECULAR);
			if (mat.map_spec_highlight) mat.map_spec_highlight->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_SPEC_HIGHLIGHT);
			if (mat.map_alpha)          mat.map_alpha->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_ALPHA);
			if (mat.map_bump)           mat.map_bump->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_NORMAL);


			// Draw the child
			model.Draw(device_context.Get(), child.GetIndexCount(), child.GetIndexStart());


			// Unbind the SRVs
			// Slot definition could be used as a more dynamic way of unbinding any amount of srvs
			// E.g. null_srv[SLOT_SRV_ALPHA + 1] = { nullptr };
			ID3D11ShaderResourceView* null_srv[6] = { nullptr };
			Pipeline::PS::BindSRVs(device_context.Get(), 0, 6, null_srv);
		});
	});
}