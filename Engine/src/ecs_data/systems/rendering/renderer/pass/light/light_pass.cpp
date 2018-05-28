#include "stdafx.h"
#include "light_pass.h"
#include "engine\engine.h"


LightPass::LightPass(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context)
	, depth_pass(make_unique<DepthPass>(device, device_context))

	, light_buffer(device)
	, directional_lights(device, 16)
	, point_lights(device, 16)
	, spot_lights(device, 16)

	, shadowed_directional_lights(device, 1)
	, shadowed_point_lights(device, 1)
	, shadowed_spot_lights(device, 1)

	, directional_light_smaps(make_unique<ShadowMapBuffer>(device, 1))
	, point_light_smaps(make_unique<ShadowCubeMapBuffer>(device, 1))
	, spot_light_smaps(make_unique<ShadowMapBuffer>(device, 1)) {
}


void XM_CALLCONV LightPass::Render(const Engine& engine, FXMMATRIX world_to_projection) {

	auto& ecs_engine = engine.GetECS();
	auto& scene      = engine.GetScene();


	// Update light buffers
	UpdateDirectionalLightData(ecs_engine, world_to_projection);
	UpdatePointLightData(ecs_engine, world_to_projection);
	UpdateSpotLightData(ecs_engine, world_to_projection);

	// Update the shadow map sizes
	UpdateShadowMaps();
	// Render the shadow maps
	RenderShadowMaps(engine);

	// Update light info buffer
	UpdateData(ecs_engine, scene);

	// Bind the buffers
	BindBuffers();
}


void LightPass::BindBuffers() {
	// Bind null RTV and DSV
	Pipeline::OM::BindRTVs(device_context, 1, nullptr, nullptr);

	// Bind light info buffer
	light_buffer.Bind<Pipeline::PS>(device_context, SLOT_CBUFFER_LIGHT);


	// Bind structured buffer SRVs
	directional_lights.Bind<Pipeline::PS>(device_context, SLOT_SRV_DIRECTIONAL_LIGHTS);
	point_lights.Bind<Pipeline::PS>(device_context, SLOT_SRV_POINT_LIGHTS);
	spot_lights.Bind<Pipeline::PS>(device_context, SLOT_SRV_SPOT_LIGHTS);

	shadowed_directional_lights.Bind<Pipeline::PS>(device_context, SLOT_SRV_DIRECTIONAL_LIGHTS_SHADOWED);
	shadowed_point_lights.Bind<Pipeline::PS>(device_context, SLOT_SRV_POINT_LIGHTS_SHADOWED);
	shadowed_spot_lights.Bind<Pipeline::PS>(device_context, SLOT_SRV_SPOT_LIGHTS_SHADOWED);


	// Bind the shadow buffer SRVs
	Pipeline::PS::BindSRVs(device_context, SLOT_SRV_DIRECTIONAL_LIGHT_SHADOW_MAPS, 1, directional_light_smaps->GetSRVAddress());
	Pipeline::PS::BindSRVs(device_context, SLOT_SRV_POINT_LIGHT_SHADOW_MAPS, 1, point_light_smaps->GetSRVAddress());
	Pipeline::PS::BindSRVs(device_context, SLOT_SRV_SPOT_LIGHT_SHADOW_MAPS, 1, spot_light_smaps->GetSRVAddress());
}


void LightPass::UpdateData(ECS& ecs_engine, Scene& scene) const {

	LightBuffer light_data;

	light_data.num_directional_lights = static_cast<u32>(directional_lights.size());
	light_data.num_point_lights       = static_cast<u32>(point_lights.size());
	light_data.num_spot_lights        = static_cast<u32>(spot_lights.size());

	light_data.num_shadow_directional_lights = static_cast<u32>(shadowed_directional_lights.size());
	light_data.num_shadow_point_lights       = static_cast<u32>(shadowed_point_lights.size());
	light_data.num_shadow_spot_lights        = static_cast<u32>(shadowed_spot_lights.size());

	light_data.fog_color = scene.GetFog().color;
	light_data.fog_start = scene.GetFog().start;
	light_data.fog_range = scene.GetFog().range;

	light_buffer.UpdateData(device_context, light_data);
}


void LightPass::UpdateShadowMaps() {

	// Clear SRVs
	ID3D11ShaderResourceView* const srvs[3] = {};
	Pipeline::PS::BindSRVs(device_context, SLOT_SRV_DIRECTIONAL_LIGHT_SHADOW_MAPS, 3, srvs);


	// Directional Lights
	{
		const size_t size      = shadowed_directional_lights.size();
		const size_t available = directional_light_smaps->GetMapCount();

		if (size > available) {
			directional_light_smaps = make_unique<ShadowMapBuffer>(device, static_cast<u32>(size));
		}

		directional_light_smaps->Clear(device_context);
	}

	// Point Lights
	{
		const size_t size      = shadowed_point_lights.size();
		const size_t available = point_light_smaps->GetMapCount();

		if (size > available) {
			point_light_smaps = make_unique<ShadowCubeMapBuffer>(device, static_cast<u32>(size));
		}

		point_light_smaps->Clear(device_context);
	}

	// Spot Lights
	{
		const size_t size      = shadowed_spot_lights.size();
		const size_t available = spot_light_smaps->GetMapCount();

		if (size > available) {
			spot_light_smaps = make_unique<ShadowMapBuffer>(device, static_cast<u32>(size));
		}

		spot_light_smaps->Clear(device_context);
	}
}


void XM_CALLCONV LightPass::UpdateDirectionalLightData(ECS& ecs_engine, FXMMATRIX world_to_projection) {

	// Temporary buffers
	vector<DirectionalLightBuffer> buffers;
	buffers.reserve(directional_lights.size());

	vector<ShadowedDirectionalLightBuffer> shadow_buffers;
	buffers.reserve(shadowed_directional_lights.size());

	// Clear the cameras
	directional_light_cameras.clear();

	ecs_engine.ForEachActive<DirectionalLight>([&](DirectionalLight& light) {

		const auto transform = ecs_engine.GetComponent<Transform>(light.GetOwner());

		if (!light.CastsShadows()) {
			DirectionalLightBuffer buffer;

			XMStoreFloat3(&buffer.direction, transform->GetWorldAxisZ());
			buffer.ambient_color = light.GetAmbientColor();
			buffer.diffuse_color = light.GetDiffuseColor();
			buffer.specular      = light.GetSpecular();

			buffers.push_back(std::move(buffer));
		}
		else {
			ShadowedDirectionalLightBuffer buffer;

			XMStoreFloat3(&buffer.light_buffer.direction, transform->GetWorldAxisZ());
			buffer.light_buffer.ambient_color = light.GetAmbientColor();
			buffer.light_buffer.diffuse_color = light.GetDiffuseColor();
			buffer.light_buffer.specular      = light.GetSpecular();

			shadow_buffers.push_back(std::move(buffer));
		}
	});

	// Update the buffers
	directional_lights.UpdateData(device, device_context, buffers);
	shadowed_directional_lights.UpdateData(device, device_context, shadow_buffers);
}


void XM_CALLCONV LightPass::UpdatePointLightData(ECS& ecs_engine, FXMMATRIX world_to_projection) {

	// Temporary buffers
	vector<PointLightBuffer> buffers;
	buffers.reserve(point_lights.size());

	vector<ShadowedPointLightBuffer> shadow_buffers;
	shadow_buffers.reserve(shadowed_point_lights.size());

	// Clear the point light cameras
	point_light_cameras.clear();


	ecs_engine.ForEachActive<PointLight>([&](PointLight& light) {

		const auto transform           = ecs_engine.GetComponent<Transform>(light.GetOwner());
		const auto light_to_world      = transform->GetObjectToWorldMatrix();
		const auto light_to_projection = light_to_world * world_to_projection;

		// Camera rotations for the cube map
		static const XMMATRIX rotations[6] = {
			XMMatrixRotationY(-XM_PIDIV2),
			XMMatrixRotationY(XM_PIDIV2),
			XMMatrixRotationX(XM_PIDIV2),
			XMMatrixRotationX(-XM_PIDIV2),
			XMMatrixIdentity(),
			XMMatrixRotationY(XM_PI)
		};

		if (!Frustum(light_to_projection).Contains(light.GetBoundingSphere()))
			return;

		if (!light.CastsShadows()) {
			PointLightBuffer buffer;

			XMStoreFloat3(&buffer.position, transform->GetWorldOrigin());
			buffer.ambient_color = light.GetAmbientColor();
			buffer.diffuse_color = light.GetDiffuseColor();
			buffer.specular      = light.GetSpecular();
			buffer.attenuation   = light.GetAttenuation();
			buffer.range         = light.GetRange();

			buffers.push_back(std::move(buffer));
		}
		else {
			const auto world_to_light       = transform->GetWorldToObjectMatrix();
			const auto light_to_lprojection = light.GetLightToProjectionMatrix();

			// Create the cameras
			for (size_t i = 0; i < 6; ++i) {
				LightCamera cam;
				cam.world_to_light = world_to_light * rotations[i];
				cam.light_to_proj  = light_to_lprojection;

				point_light_cameras.push_back(std::move(cam));
			}

			// Create the buffer
			ShadowedPointLightBuffer buffer;
			
			XMStoreFloat3(&buffer.light_buffer.position, transform->GetWorldOrigin());
			buffer.light_buffer.ambient_color = light.GetAmbientColor();
			buffer.light_buffer.diffuse_color = light.GetDiffuseColor();
			buffer.light_buffer.specular      = light.GetSpecular();
			buffer.light_buffer.attenuation   = light.GetAttenuation();
			buffer.light_buffer.range         = light.GetRange();

			buffer.world_to_light    = XMMatrixTranspose(world_to_light);

			float2 proj_values = { XMVectorGetZ(light_to_lprojection.r[2]), XMVectorGetZ(light_to_lprojection.r[3]) };
			buffer.projection_values = proj_values;

			shadow_buffers.push_back(std::move(buffer));
		}
	});

	// Update the buffers
	point_lights.UpdateData(device, device_context, buffers);
	shadowed_point_lights.UpdateData(device, device_context, shadow_buffers);
}


void XM_CALLCONV LightPass::UpdateSpotLightData(ECS& ecs_engine, FXMMATRIX world_to_projection) {

	// Temporary buffer vectors
	vector<SpotLightBuffer> buffers;
	buffers.reserve(spot_lights.size());

	vector<ShadowedSpotLightBuffer> shadow_buffers;
	shadow_buffers.reserve(spot_lights.size());

	// Clear the spot light cameras
	spot_light_cameras.clear();


	ecs_engine.ForEachActive<SpotLight>([&](SpotLight& light) {

		const auto transform           = ecs_engine.GetComponent<Transform>(light.GetOwner());
		const auto light_to_world      = transform->GetObjectToWorldMatrix();
		const auto light_to_projection = light_to_world * world_to_projection;

		if (!Frustum(light_to_projection).Contains(light.GetAABB()))
			return;

		if (!light.CastsShadows()) {
			SpotLightBuffer buffer;

			XMStoreFloat3(&buffer.position, transform->GetWorldOrigin());
			XMStoreFloat3(&buffer.direction, transform->GetWorldAxisZ());
			buffer.ambient_color = light.GetAmbientColor();
			buffer.diffuse_color = light.GetDiffuseColor();
			buffer.specular      = light.GetSpecular();
			buffer.attenuation   = light.GetAttenuation();
			buffer.cos_umbra     = light.GetUmbra();
			buffer.cos_penumbra  = light.GetPenumbra();
			buffer.range         = light.GetRange();

			buffers.push_back(std::move(buffer));
		}
		else {
			const auto world_to_light       = transform->GetWorldToObjectMatrix();
			const auto light_to_lprojection = light.GetLightToProjectionMatrix();
			const auto world_to_lprojection = world_to_light * light_to_lprojection;

			// Create the camera
			LightCamera cam;
			cam.world_to_light = world_to_light;
			cam.light_to_proj  = light_to_lprojection;

			spot_light_cameras.push_back(std::move(cam));


			// Create the buffer
			ShadowedSpotLightBuffer buffer;

			XMStoreFloat3(&buffer.light_buffer.position, transform->GetWorldOrigin());
			XMStoreFloat3(&buffer.light_buffer.direction, transform->GetWorldAxisZ());
			buffer.light_buffer.ambient_color = light.GetAmbientColor();
			buffer.light_buffer.diffuse_color = light.GetDiffuseColor();
			buffer.light_buffer.specular      = light.GetSpecular();
			buffer.light_buffer.attenuation   = light.GetAttenuation();
			buffer.light_buffer.range         = light.GetRange();
			buffer.light_buffer.cos_umbra     = light.GetUmbra();
			buffer.light_buffer.cos_penumbra  = light.GetPenumbra();

			buffer.world_to_projection = XMMatrixTranspose(world_to_lprojection);

			shadow_buffers.push_back(std::move(buffer));
		}
	});

	// Update buffers
	spot_lights.UpdateData(device, device_context, buffers);
	shadowed_spot_lights.UpdateData(device, device_context, shadow_buffers);
}


void LightPass::RenderShadowMaps(const Engine& engine) {

	depth_pass->BindState(engine.GetRenderingMgr().GetRenderStateMgr());


	// Directional Lights
	{
		directional_light_smaps->BindViewport(device_context);
		directional_light_smaps->BindRasterState(device_context);

		size_t i = 0;
		for (const auto& camera : directional_light_cameras) {
			directional_light_smaps->BindDSV(device_context, i++);
			depth_pass->RenderShadows(engine, camera.world_to_light, camera.light_to_proj);
		}
	}


	// Point Lights
	{
		point_light_smaps->BindViewport(device_context);
		point_light_smaps->BindRasterState(device_context);

		size_t i = 0;
		for (const auto& camera : point_light_cameras) {
			point_light_smaps->BindDSV(device_context, i++);
			depth_pass->RenderShadows(engine, camera.world_to_light, camera.light_to_proj);
		}
	}


	// Spot Lights
	{
		spot_light_smaps->BindViewport(device_context);
		spot_light_smaps->BindRasterState(device_context);

		size_t i = 0;
		for (const auto& camera : spot_light_cameras) {
			spot_light_smaps->BindDSV(device_context, i++);
			depth_pass->RenderShadows(engine, camera.world_to_light, camera.light_to_proj);
		}
	}
}