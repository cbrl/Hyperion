#include "stdafx.h"
#include "forward_render.h"
#include "rendering\rendering_mgr.h"
#include "system\system.h"


ForwardRenderer::ForwardRenderer()
	: model_buffer(RenderingMgr::GetDevice())
	, light_buffer(RenderingMgr::GetDevice())
	, directional_light_buffer(RenderingMgr::GetDevice(), 16)
	, point_light_buffer(RenderingMgr::GetDevice(), 16)
	, spot_light_buffer(RenderingMgr::GetDevice(), 16)
{
	vertex_shader = make_unique<VertexShader>();
	vertex_shader->Init(System::Get()->GetHWND(), RenderingMgr::GetDevice(), L"shaders/forward/forward_VS.hlsl",
						VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount);

	pixel_shader = make_unique<PixelShader>();
	pixel_shader->Init(System::Get()->GetHWND(), RenderingMgr::GetDevice(), L"shaders/forward/forward.hlsl");


	auto device_context = RenderingMgr::GetDeviceContext();

	// Set buffers
	/***/device_context->PSSetConstantBuffers(SLOT_CBUFFER_MODEL, 1, model_buffer.GetBufferAddress());
	/***/device_context->VSSetConstantBuffers(SLOT_CBUFFER_MODEL, 1, model_buffer.GetBufferAddress());
	device_context->PSSetConstantBuffers(SLOT_CBUFFER_LIGHT, 1, light_buffer.GetBufferAddress());
	device_context->PSSetShaderResources(SLOT_SRV_DIRECTIONAL_LIGHTS, 1, directional_light_buffer.GetSRVAddress());
	device_context->PSSetShaderResources(SLOT_SRV_POINT_LIGHTS, 1, point_light_buffer.GetSRVAddress());
	device_context->PSSetShaderResources(SLOT_SRV_SPOT_LIGHTS, 1, spot_light_buffer.GetSRVAddress());
}


void ForwardRenderer::Render(Scene& scene) {

	auto device         = RenderingMgr::GetDevice();
	auto device_context = RenderingMgr::GetDeviceContext();

	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------

	RenderStateMgr::Get()->BindDepthDefault(device_context);


	XMMATRIX world      = Direct3D::Get()->GetWorldMatrix();
	XMMATRIX view       = scene.camera->GetViewMatrix();
	XMMATRIX projection = Direct3D::Get()->GetProjectionMatrix();


	// Create frustum
	Frustum frustum(view*projection);


	// Bind shaders
	pixel_shader->Bind(device_context);
	vertex_shader->Bind(device_context);


	// Update light buffer
	LightBuffer light_data;
	light_data.directional_light_count = static_cast<uint32_t>(scene.directional_lights.size());
	light_data.point_light_count       = static_cast<uint32_t>(scene.point_lights.size());
	light_data.spot_light_count        = static_cast<uint32_t>(scene.spot_lights.size());
	//light_data.fog_color = ; //fog memeber in scene?
	//light_data.fog_start = ;
	//light_data.fog_range = ;
	light_buffer.UpdateData(device_context, light_data);


	// Update light data buffers
	if (scene.directional_lights.size() > 0)
		directional_light_buffer.UpdateData(device, device_context, scene.directional_lights);

	if (scene.point_lights.size() > 0)
		point_light_buffer.UpdateData(device, device_context, scene.point_lights);

	if (scene.spot_lights.size() > 0)
		spot_light_buffer.UpdateData(device, device_context, scene.spot_lights);
	

	// Render models
	scene.ForEach<Model>([&](const Model& model) {

		if (frustum.Contains(model.GetAABB())) {

			// Create the world matrix
			world = Direct3D::Get()->GetWorldMatrix();
			world = XMMatrixMultiply(world, model.GetScale());
			world = XMMatrixMultiply(world, model.GetRotation());
			world = XMMatrixMultiply(world, model.GetPosition());

			// Create the inverse transpose of the world matrix
			XMMATRIX inv_transpose = InverseTranspose(world);


			// Render each model part individually
			model.ForEachPart([&](const ModelPart& part) {

				// Update model buffer
				ModelBuffer model_data;
				model_data.world = world;
				model_data.world_inv_transpose = inv_transpose;
				model_data.world_view_proj = XMMatrixTranspose(world * view * projection);
				model_data.texTransform = XMMatrixIdentity();	//Replace with actual texTransform

				// Get the material for this model part
				auto mat = model.GetMaterial(part.material_index);

				// Set the material parameters in the model buffer
				MaterialBuffer temp_mat = { mat.Ka, mat.Kd, mat.Ks, mat.Ke, mat.Ns, mat.Ni, mat.d };
				model_data.mat = temp_mat;

				// Update the model buffer
				model_buffer.UpdateData(device_context, model_data);


				// Bind the SRVs
				if (mat.map_Kd)   mat.map_Kd->Bind(device_context, SLOT_SRV_DIFFUSE);
				if (mat.map_Ka)   mat.map_Ka->Bind(device_context, SLOT_SRV_AMBIENT);
				if (mat.map_Ks)   mat.map_Ks->Bind(device_context, SLOT_SRV_SPECULAR);
				if (mat.map_Ns)   mat.map_Ns->Bind(device_context, SLOT_SRV_SPEC_HIGHLIGHT);
				if (mat.map_d)    mat.map_d->Bind(device_context, SLOT_SRV_ALPHA);
				if (mat.map_bump) mat.map_bump->Bind(device_context, SLOT_SRV_BUMP);


				// Draw the model part
				model.Draw(device_context, part.index_count, part.index_start);
			});
		}
	});
}
