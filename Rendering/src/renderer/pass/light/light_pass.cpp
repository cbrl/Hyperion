#include "light_pass.h"

#include "hlsl.h"
#include "rendering_config.h"
#include "scene/scene.h"
#include "renderer/state/render_state_mgr.h"
#include "resource/resource_mgr.h"
#include "geometry/frustum/frustum.h"


LightPass::LightPass(const RenderingConfig& rendering_config,
                     ID3D11Device& device,
	                 ID3D11DeviceContext& device_context,
	                 RenderStateMgr& render_state_mgr,
	                 ResourceMgr& resource_mgr)
	: device(device)
	, device_context(device_context)
	, rendering_config(rendering_config)
	, depth_pass(std::make_unique<DepthPass>(device, device_context, render_state_mgr, resource_mgr))

	, light_buffer(device)
	, directional_lights(device, 16)
	, point_lights(device, 16)
	, spot_lights(device, 16)

	, shadowed_directional_lights(device, 1)
	, shadowed_point_lights(device, 1)
	, shadowed_spot_lights(device, 1)

	, directional_light_smaps(std::make_unique<ShadowMapBuffer>(device, 1, rendering_config.getShadowMapRes()))
	, point_light_smaps(std::make_unique<ShadowCubeMapBuffer>(device, 1, rendering_config.getShadowMapRes()))
	, spot_light_smaps(std::make_unique<ShadowMapBuffer>(device, 1, rendering_config.getShadowMapRes())) {
}


void XM_CALLCONV LightPass::render(Scene& scene, FXMMATRIX world_to_projection) {

	// Update light buffers
	updateDirectionalLightData(scene, world_to_projection);
	updatePointLightData(scene, world_to_projection);
	updateSpotLightData(scene, world_to_projection);

	// Update the shadow std::map sizes
	updateShadowMaps();
	// Render the shadow maps
	renderShadowMaps(scene);

	// Update light info buffer
	updateData(scene);

	// Bind the buffers
	bindBuffers();
}


void LightPass::bindBuffers() {

	// Bind null RTV and DSV
	Pipeline::OM::bindRTVsAndDSV(device_context, 0, nullptr, nullptr);

	// Bind light info buffer
	light_buffer.bind<Pipeline::PS>(device_context, SLOT_CBUFFER_LIGHT);


	// Bind structured buffer SRVs
	directional_lights.bind<Pipeline::PS>(device_context, SLOT_SRV_DIRECTIONAL_LIGHTS);
	point_lights.bind<Pipeline::PS>(device_context, SLOT_SRV_POINT_LIGHTS);
	spot_lights.bind<Pipeline::PS>(device_context, SLOT_SRV_SPOT_LIGHTS);

	shadowed_directional_lights.bind<Pipeline::PS>(device_context, SLOT_SRV_DIRECTIONAL_LIGHTS_SHADOWED);
	shadowed_point_lights.bind<Pipeline::PS>(device_context, SLOT_SRV_POINT_LIGHTS_SHADOWED);
	shadowed_spot_lights.bind<Pipeline::PS>(device_context, SLOT_SRV_SPOT_LIGHTS_SHADOWED);


	// Bind the shadow buffer SRVs
	Pipeline::PS::bindSRVs(device_context, SLOT_SRV_DIRECTIONAL_LIGHT_SHADOW_MAPS, 1, directional_light_smaps->getSRVAddress());
	Pipeline::PS::bindSRVs(device_context, SLOT_SRV_POINT_LIGHT_SHADOW_MAPS,       1, point_light_smaps->getSRVAddress());
	Pipeline::PS::bindSRVs(device_context, SLOT_SRV_SPOT_LIGHT_SHADOW_MAPS,        1, spot_light_smaps->getSRVAddress());
}


void LightPass::updateData(Scene& scene) const {

	LightBuffer light_data;

	light_data.num_directional_lights = static_cast<u32>(directional_lights.size());
	light_data.num_point_lights       = static_cast<u32>(point_lights.size());
	light_data.num_spot_lights        = static_cast<u32>(spot_lights.size());

	light_data.num_shadow_directional_lights = static_cast<u32>(shadowed_directional_lights.size());
	light_data.num_shadow_point_lights       = static_cast<u32>(shadowed_point_lights.size());
	light_data.num_shadow_spot_lights        = static_cast<u32>(shadowed_spot_lights.size());

	scene.forEach<AmbientLight>([&light_data](const AmbientLight& light) {
		if (!light.isActive()) return;
		light_data.ambient += light.getColor();
	});

	light_buffer.updateData(device_context, light_data);
}


void LightPass::updateShadowMaps() {

	// Clear SRVs
	ID3D11ShaderResourceView* const srvs[3] = {};
	Pipeline::PS::bindSRVs(device_context, SLOT_SRV_DIRECTIONAL_LIGHT_SHADOW_MAPS, 3, srvs);


	// Directional Lights
	{
		const size_t size      = shadowed_directional_lights.size();
		const size_t available = directional_light_smaps->getMapCount();

		const auto curr_res   = directional_light_smaps->getMapRes();
		const auto config_res = rendering_config.getShadowMapRes();


		if (size > available || curr_res != config_res) {
			u32 count = size ? static_cast<u32>(size) : 1;
			directional_light_smaps = std::make_unique<ShadowMapBuffer>(device, count, config_res);
		}

		directional_light_smaps->clear(device_context);
	}

	// Point Lights
	{
		const size_t size      = shadowed_point_lights.size();
		const size_t available = point_light_smaps->getMapCount();

		const auto curr_res   = point_light_smaps->getMapRes();
		const auto config_res = rendering_config.getShadowMapRes();

		if (size > available || curr_res != config_res) {
			u32 count = size ? static_cast<u32>(size) : 1;
			point_light_smaps = std::make_unique<ShadowCubeMapBuffer>(device, count, config_res);
		}

		point_light_smaps->clear(device_context);
	}

	// Spot Lights
	{
		const size_t size      = shadowed_spot_lights.size();
		const size_t available = spot_light_smaps->getMapCount();

		const auto curr_res   = spot_light_smaps->getMapRes();
		const auto config_res = rendering_config.getShadowMapRes();

		if (size > available || curr_res != config_res) {
			u32 count = size ? static_cast<u32>(size) : 1;
			spot_light_smaps = std::make_unique<ShadowMapBuffer>(device, count, config_res);
		}

		spot_light_smaps->clear(device_context);
	}
}


void XM_CALLCONV LightPass::updateDirectionalLightData(Scene& scene, FXMMATRIX world_to_projection) {

	// Temporary buffers
	std::vector<DirectionalLightBuffer> buffers;
	buffers.reserve(directional_lights.size());

	std::vector<DirectionalLightBuffer> shadow_buffers;
	buffers.reserve(shadowed_directional_lights.size());

	// Clear the cameras
	directional_light_cameras.clear();

	scene.forEach<DirectionalLight>([&](DirectionalLight& light) {

		if (!light.isActive()) return;

		const auto transform = light.getOwner()->getComponent<Transform>();
		if (!transform) return;

		const auto light_to_world      = transform->getObjectToWorldMatrix();
		const auto light_to_projection = light_to_world * world_to_projection;

		if (!Frustum(light_to_projection).contains(light.getAABB()))
			return;

		const auto world_to_light       = transform->getWorldToObjectMatrix();
		const auto light_to_lprojection = light.getLightToProjectionMatrix();
		const auto world_to_lprojection = world_to_light * light_to_lprojection;

		DirectionalLightBuffer buffer;
		XMStore(&buffer.direction, transform->getWorldAxisZ());
		buffer.diffuse_color       = light.getDiffuseColor();
		buffer.specular            = light.getSpecular();
		buffer.world_to_projection = XMMatrixTranspose(world_to_lprojection);

		if (light.castsShadows()) {
			LightCamera cam;
			cam.world_to_light = world_to_light;
			cam.light_to_proj = light_to_lprojection;

			directional_light_cameras.push_back(std::move(cam));
			shadow_buffers.push_back(std::move(buffer));
		}
		else {
			buffers.push_back(std::move(buffer));
		}
	});

	// Update the buffers
	directional_lights.updateData(device, device_context, buffers);
	shadowed_directional_lights.updateData(device, device_context, shadow_buffers);
}


void XM_CALLCONV LightPass::updatePointLightData(Scene& scene, FXMMATRIX world_to_projection) {

	// Temporary buffers
	std::vector<PointLightBuffer> buffers;
	buffers.reserve(point_lights.size());

	std::vector<ShadowedPointLightBuffer> shadow_buffers;
	shadow_buffers.reserve(shadowed_point_lights.size());

	// Clear the point light cameras
	point_light_cameras.clear();


	scene.forEach<PointLight>([&](PointLight& light) {

		if (!light.isActive()) return;

		const auto transform = light.getOwner()->getComponent<Transform>();
		if (!transform) return;

		const auto light_to_world      = transform->getObjectToWorldMatrix();
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

		if (!Frustum(light_to_projection).contains(light.getBoundingSphere()))
			return;

		if (light.castsShadows()) {
			const auto world_to_light = transform->getWorldToObjectMatrix();
			const auto light_to_lprojection = light.getLightToProjectionMatrix();

			// Create the cameras
			for (size_t i = 0; i < 6; ++i) {
				LightCamera cam;
				cam.world_to_light = world_to_light * rotations[i];
				cam.light_to_proj = light_to_lprojection;

				point_light_cameras.push_back(std::move(cam));
			}

			// Create the buffer
			ShadowedPointLightBuffer buffer;

			XMStore(&buffer.light_buffer.position, transform->getWorldOrigin());
			buffer.light_buffer.diffuse_color = light.getDiffuseColor();
			buffer.light_buffer.specular      = light.getSpecular();
			buffer.light_buffer.attenuation   = light.getAttenuation();
			buffer.light_buffer.range         = light.getRange();

			buffer.world_to_light = XMMatrixTranspose(world_to_light);

			const vec2_f32 proj_values = {
				XMVectorGetZ(light_to_lprojection.r[2]),
				XMVectorGetZ(light_to_lprojection.r[3])
			};
			buffer.projection_values = proj_values;

			shadow_buffers.push_back(std::move(buffer));
		}
		else {
			PointLightBuffer buffer;

			XMStore(&buffer.position, transform->getWorldOrigin());
			buffer.diffuse_color = light.getDiffuseColor();
			buffer.specular      = light.getSpecular();
			buffer.attenuation   = light.getAttenuation();
			buffer.range         = light.getRange();

			buffers.push_back(std::move(buffer));
		}
	});

	// Update the buffers
	point_lights.updateData(device, device_context, buffers);
	shadowed_point_lights.updateData(device, device_context, shadow_buffers);
}


void XM_CALLCONV LightPass::updateSpotLightData(Scene& scene, FXMMATRIX world_to_projection) {

	// Temporary buffer vectors
	std::vector<SpotLightBuffer> buffers;
	buffers.reserve(spot_lights.size());

	std::vector<ShadowedSpotLightBuffer> shadow_buffers;
	shadow_buffers.reserve(spot_lights.size());

	// Clear the spot light cameras
	spot_light_cameras.clear();


	scene.forEach<SpotLight>([&](SpotLight& light) {

		if (!light.isActive()) return;

		const auto transform = light.getOwner()->getComponent<Transform>();
		if (!transform) return;

		const auto light_to_world      = transform->getObjectToWorldMatrix();
		const auto light_to_projection = light_to_world * world_to_projection;

		if (!Frustum(light_to_projection).contains(light.getAABB()))
			return;

		if (light.castsShadows()) {
			const auto world_to_light       = transform->getWorldToObjectMatrix();
			const auto light_to_lprojection = light.getLightToProjectionMatrix();
			const auto world_to_lprojection = world_to_light * light_to_lprojection;

			// Create the camera
			LightCamera cam;
			cam.world_to_light = world_to_light;
			cam.light_to_proj  = light_to_lprojection;

			spot_light_cameras.push_back(std::move(cam));


			// Create the buffer
			ShadowedSpotLightBuffer buffer;

			XMStore(&buffer.light_buffer.position, transform->getWorldOrigin());
			XMStore(&buffer.light_buffer.direction, transform->getWorldAxisZ());
			buffer.light_buffer.diffuse_color = light.getDiffuseColor();
			buffer.light_buffer.specular      = light.getSpecular();
			buffer.light_buffer.attenuation   = light.getAttenuation();
			buffer.light_buffer.range         = light.getRange();
			buffer.light_buffer.cos_umbra     = light.getUmbra();
			buffer.light_buffer.cos_penumbra  = light.getPenumbra();

			buffer.world_to_projection = XMMatrixTranspose(world_to_lprojection);

			shadow_buffers.push_back(std::move(buffer));
		}
		else {
			SpotLightBuffer buffer;

			XMStore(&buffer.position, transform->getWorldOrigin());
			XMStore(&buffer.direction, transform->getWorldAxisZ());
			buffer.diffuse_color = light.getDiffuseColor();
			buffer.specular      = light.getSpecular();
			buffer.attenuation   = light.getAttenuation();
			buffer.cos_umbra     = light.getUmbra();
			buffer.cos_penumbra  = light.getPenumbra();
			buffer.range         = light.getRange();

			buffers.push_back(std::move(buffer));
		}
	});

	// Update buffers
	spot_lights.updateData(device, device_context, buffers);
	shadowed_spot_lights.updateData(device, device_context, shadow_buffers);
}


void LightPass::renderShadowMaps(Scene& scene) {

	depth_pass->bindState();

	// Directional Lights
	{
		directional_light_smaps->bindViewport(device_context);
		directional_light_smaps->bindRasterState(device_context);

		size_t i = 0;
		for (const auto& camera : directional_light_cameras) {
			directional_light_smaps->bindDSV(device_context, i++);
			depth_pass->renderShadows(scene, camera.world_to_light, camera.light_to_proj);
		}
	}


	// Point Lights
	{
		point_light_smaps->bindViewport(device_context);
		point_light_smaps->bindRasterState(device_context);

		size_t i = 0;
		for (const auto& camera : point_light_cameras) {
			point_light_smaps->bindDSV(device_context, i++);
			depth_pass->renderShadows(scene, camera.world_to_light, camera.light_to_proj);
		}
	}


	// Spot Lights
	{
		spot_light_smaps->bindViewport(device_context);
		spot_light_smaps->bindRasterState(device_context);

		size_t i = 0;
		for (const auto& camera : spot_light_cameras) {
			spot_light_smaps->bindDSV(device_context, i++);
			depth_pass->renderShadows(scene, camera.world_to_light, camera.light_to_proj);
		}
	}
}
