#include "stdafx.h"
#include "forward_render.h"


ForwardRenderer::ForwardRenderer(ID3D11Device* device, ID3D11DeviceContext* device_context)
	: device(device)
	, device_context(device_context)
	, model_buffer(device)
	, light_buffer(device)
	, directional_light_buffer(device, 16)
	, point_light_buffer(device, 16)
	, spot_light_buffer(device, 16)
{
	// Create the vertex shader
	vertex_shader = make_unique<VertexShader>(device, L"shaders/forward/forward_VS.hlsl",
											  VertexPositionNormalTexture::InputElements,
											  VertexPositionNormalTexture::InputElementCount);

	// Create the pixel shader
	pixel_shader = make_unique<PixelShader>(device, L"shaders/forward/forward.hlsl");


	// Bind buffers
	model_buffer.Bind<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);
	model_buffer.Bind<Pipeline::PS>(device_context, SLOT_CBUFFER_MODEL);
	light_buffer.Bind<Pipeline::PS>(device_context, SLOT_CBUFFER_LIGHT);

	// Bind SRVs
	directional_light_buffer.Bind<Pipeline::PS>(device_context, SLOT_SRV_DIRECTIONAL_LIGHTS);
	point_light_buffer.Bind<Pipeline::PS>(device_context, SLOT_SRV_POINT_LIGHTS);
	spot_light_buffer.Bind<Pipeline::PS>(device_context, SLOT_SRV_SPOT_LIGHTS);
}


void ForwardRenderer::Render(Scene& scene, RenderStateMgr& render_state_mgr) {

	//----------------------------------------------------------------------------------
	// Bind shaders
	//----------------------------------------------------------------------------------

	pixel_shader->Bind(device_context.Get());
	vertex_shader->Bind(device_context.Get());


	//----------------------------------------------------------------------------------
	// Bind the default depth render state
	//----------------------------------------------------------------------------------

	render_state_mgr.BindDepthDefault(device_context.Get());


	//----------------------------------------------------------------------------------
	// Get the matrices
	//----------------------------------------------------------------------------------

	XMMATRIX world;
	XMMATRIX view       = scene.camera->GetViewMatrix();
	XMMATRIX projection = scene.camera->GetProjMatrix();


	//----------------------------------------------------------------------------------
	// Create the frustum
	//----------------------------------------------------------------------------------

	Frustum frustum(view * projection);


	//----------------------------------------------------------------------------------
	// Update buffers
	//----------------------------------------------------------------------------------

	// Update light buffer
	LightBuffer light_data;
	light_data.directional_light_count = static_cast<u32>(scene.directional_lights.size());
	light_data.point_light_count       = static_cast<u32>(scene.point_lights.size());
	light_data.spot_light_count        = static_cast<u32>(scene.spot_lights.size());

	//fog memeber in scene?
	light_data.fog_color = float4(0.4f, 0.4f, 0.4f, 0.4f);
	light_data.fog_start = 20.0f;
	light_data.fog_range = 50.0f;

	light_buffer.UpdateData(device_context.Get(), light_data);


	// Update light data buffers
	directional_light_buffer.UpdateData(device.Get(), device_context.Get(), scene.directional_lights);
	point_light_buffer.UpdateData(device.Get(),       device_context.Get(), scene.point_lights);
	spot_light_buffer.UpdateData(device.Get(),        device_context.Get(), scene.spot_lights);
	

	//----------------------------------------------------------------------------------
	// Render models
	//----------------------------------------------------------------------------------

	scene.ForEach<Model>([&](const Model& model) {

		// Cull models that aren't on screen
		if (!frustum.Contains(model.GetAABB())) return;

		// Create the world matrix
		world = XMMatrixIdentity();
		world = XMMatrixMultiply(world, model.GetScale());
		world = XMMatrixMultiply(world, model.GetRotation());
		world = XMMatrixMultiply(world, model.GetPosition());

		// Create the inverse transpose of the world matrix.
		// DXMath uses row major matrices, unlike HLSL, so
		// the transpose is implied when sending it to the shader.
		XMMATRIX inv_transpose = XMMatrixInverse(NULL, world);


		// Render each model part individually
		model.ForEachPart([&](const ModelPart& part) {

			// Cull model parts that aren't on screen
			if (!frustum.Contains(part.aabb)) return;

			// Update model buffer
			ModelBuffer model_data;
			model_data.world               = XMMatrixTranspose(world);
			model_data.world_inv_transpose = inv_transpose;
			model_data.world_view_proj     = XMMatrixTranspose(world * view * projection);
			model_data.texTransform        = XMMatrixIdentity();	//Replace with actual texTransform

			// Get a pointer to the material for this model part
			auto mat = &model.GetMaterial(part.material_index);

			// Set the material parameters in the model buffer
			model_data.mat.ambient         = mat->Ka;
			model_data.mat.diffuse         = mat->Kd;
			model_data.mat.dissolve        = mat->d;
			model_data.mat.emissive        = mat->Ke;
			model_data.mat.optical_density = mat->Ni;
			model_data.mat.specular        = mat->Ks;

			// Update the model buffer
			model_buffer.UpdateData(device_context.Get(), model_data);


			// Bind the SRVs
			if (mat->map_Kd)   mat->map_Kd->Bind<Pipeline::PS>(device_context.Get(),   SLOT_SRV_DIFFUSE);
			if (mat->map_Ka)   mat->map_Ka->Bind<Pipeline::PS>(device_context.Get(),   SLOT_SRV_AMBIENT);
			if (mat->map_Ks)   mat->map_Ks->Bind<Pipeline::PS>(device_context.Get(),   SLOT_SRV_SPECULAR);
			if (mat->map_Ns)   mat->map_Ns->Bind<Pipeline::PS>(device_context.Get(),   SLOT_SRV_SPEC_HIGHLIGHT);
			if (mat->map_d)    mat->map_d->Bind<Pipeline::PS>(device_context.Get(),    SLOT_SRV_ALPHA);
			if (mat->map_bump) mat->map_bump->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_BUMP);


			// Draw the model part
			model.Draw(device_context.Get(), part.index_count, part.index_start);
		});
	});
}