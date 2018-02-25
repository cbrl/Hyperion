#include "stdafx.h"
#include "forward_render.h"
#include "rendering\rendering_mgr.h"
#include "system\system.h"


ForwardRenderer::ForwardRenderer()
	: device(RenderingMgr::GetDevice())
	, device_context(RenderingMgr::GetDeviceContext())
	, model_buffer(device.Get())
	, light_buffer(device.Get())
	, directional_light_buffer(device.Get(), 16)
	, point_light_buffer(device.Get(), 16)
	, spot_light_buffer(device.Get(), 16)
{
	// Create the vertex shader
	vertex_shader = make_unique<VertexShader>();
	vertex_shader->Init(System::Get()->GetHWND(), device.Get(), L"shaders/forward/forward_VS.hlsl",
						VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount);

	// Create the pixel shader
	pixel_shader = make_unique<PixelShader>();
	pixel_shader->Init(System::Get()->GetHWND(), device.Get(), L"shaders/forward/forward.hlsl");


	// Set buffers
	/***/device_context->PSSetConstantBuffers(SLOT_CBUFFER_MODEL, 1, model_buffer.GetBufferAddress());
	/***/device_context->VSSetConstantBuffers(SLOT_CBUFFER_MODEL, 1, model_buffer.GetBufferAddress());
	device_context->PSSetConstantBuffers(SLOT_CBUFFER_LIGHT, 1, light_buffer.GetBufferAddress());
	device_context->PSSetShaderResources(SLOT_SRV_DIRECTIONAL_LIGHTS, 1, directional_light_buffer.GetSRVAddress());
	device_context->PSSetShaderResources(SLOT_SRV_POINT_LIGHTS, 1, point_light_buffer.GetSRVAddress());
	device_context->PSSetShaderResources(SLOT_SRV_SPOT_LIGHTS, 1, spot_light_buffer.GetSRVAddress());
}


void ForwardRenderer::Render(Scene& scene) {

	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------

	// Bind the default depth render state
	RenderStateMgr::Get()->BindDepthDefault(device_context.Get());


	// Get the matrices
	XMMATRIX world      = scene.camera->GetWorldMatrix();
	XMMATRIX view       = scene.camera->GetViewMatrix();
	XMMATRIX projection = scene.camera->GetProjMatrix();


	// Create frustum
	Frustum frustum(view*projection);


	// Bind shaders
	pixel_shader->Bind(device_context.Get());
	vertex_shader->Bind(device_context.Get());


	// Update light buffer
	LightBuffer light_data;
	light_data.directional_light_count = static_cast<u32>(scene.directional_lights.size());
	light_data.point_light_count       = static_cast<u32>(scene.point_lights.size());
	light_data.spot_light_count        = static_cast<u32>(scene.spot_lights.size());
	//light_data.fog_color = ; //fog memeber in scene?
	//light_data.fog_start = ;
	//light_data.fog_range = ;
	light_buffer.UpdateData(device_context.Get(), light_data);


	// Update light data buffers
	directional_light_buffer.UpdateData(device.Get(), device_context.Get(), scene.directional_lights);
	
	point_light_buffer.UpdateData(device.Get(), device_context.Get(), scene.point_lights);
	
	spot_light_buffer.UpdateData(device.Get(), device_context.Get(), scene.spot_lights);
	

	// Render models
	scene.ForEach<Model>([&](const Model& model) {

		if (frustum.Contains(model.GetAABB())) {

			// Create the world matrix
			world = scene.camera->GetWorldMatrix();
			world = XMMatrixMultiply(world, model.GetScale());
			world = XMMatrixMultiply(world, model.GetRotation());
			world = XMMatrixMultiply(world, model.GetPosition());

			// Create the inverse transpose of the world matrix.
			// DXMath uses row major matrices, unlike HLSL, so
			// the transpose is implied when sending it to the shader.
			XMMATRIX inv_transpose = XMMatrixInverse(NULL, world);


			// Render each model part individually
			model.ForEachPart([&](const ModelPart& part) {

				// Update model buffer
				ModelBuffer model_data;
				model_data.world               = XMMatrixTranspose(world);
				model_data.world_inv_transpose = inv_transpose;
				model_data.world_view_proj     = XMMatrixTranspose(world * view * projection);
				model_data.texTransform        = XMMatrixIdentity();	//Replace with actual texTransform

				// Get the material for this model part
				auto mat = model.GetMaterial(part.material_index);

				// Set the material parameters in the model buffer
				model_data.mat.ambient         = mat.Ka;
				model_data.mat.diffuse         = mat.Kd;
				model_data.mat.dissolve        = mat.d;
				model_data.mat.emissive        = mat.Ke;
				model_data.mat.optical_density = mat.Ni;
				model_data.mat.specular        = mat.Ks;

				// Update the model buffer
				model_buffer.UpdateData(device_context.Get(), model_data);


				// Bind the SRVs
				if (mat.map_Kd)   mat.map_Kd->Bind(device_context.Get(),   SLOT_SRV_DIFFUSE);
				if (mat.map_Ka)   mat.map_Ka->Bind(device_context.Get(),   SLOT_SRV_AMBIENT);
				if (mat.map_Ks)   mat.map_Ks->Bind(device_context.Get(),   SLOT_SRV_SPECULAR);
				if (mat.map_Ns)   mat.map_Ns->Bind(device_context.Get(),   SLOT_SRV_SPEC_HIGHLIGHT);
				if (mat.map_d)    mat.map_d->Bind(device_context.Get(),    SLOT_SRV_ALPHA);
				if (mat.map_bump) mat.map_bump->Bind(device_context.Get(), SLOT_SRV_BUMP);


				// Draw the model part
				model.Draw(device_context.Get(), part.index_count, part.index_start);
			});
		}
	});
}
