#include "stdafx.h"
#include "light_pass.h"
#include "engine\engine.h"


LightPass::LightPass(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context)
	, light_buffer(device)
	, directional_light_buffer(device, 16)
	, point_light_buffer(device, 16)
	, spot_light_buffer(device, 16) {
}


void LightPass::Render(const Engine& engine) {

	auto& ecs_engine = engine.GetECS();
	auto& scene      = engine.GetScene();


	// Update light data buffers
	UpdateDirectionalLightData(ecs_engine);
	UpdatePointLightData(ecs_engine);
	UpdateSpotLightData(ecs_engine);


	// Update light data buffers
	directional_light_buffer.UpdateData(device, device_context, scene.GetDirectionalLights());
	point_light_buffer.UpdateData(device, device_context, scene.GetPointLights());
	spot_light_buffer.UpdateData(device, device_context, scene.GetSpotLights());


	UpdateData(ecs_engine, scene);
	BindBuffers();
}


void LightPass::BindBuffers() {

	// Bind buffer
	light_buffer.Bind<Pipeline::PS>(device_context, SLOT_CBUFFER_LIGHT);

	// Bind SRVs
	directional_light_buffer.Bind<Pipeline::PS>(device_context, SLOT_SRV_DIRECTIONAL_LIGHTS);
	point_light_buffer.Bind<Pipeline::PS>(device_context, SLOT_SRV_POINT_LIGHTS);
	spot_light_buffer.Bind<Pipeline::PS>(device_context, SLOT_SRV_SPOT_LIGHTS);
}


void LightPass::UpdateData(ECS& ecs_engine, Scene& scene) const {

	LightBuffer light_data;

	//light_data.directional_light_count = static_cast<u32>(scene.GetDirectionalLights().size());
	//light_data.point_light_count       = static_cast<u32>(scene.GetPointLights().size());
	//light_data.spot_light_count        = static_cast<u32>(scene.GetSpotLights().size());

	light_data.directional_light_count = static_cast<u32>(ecs_engine.CountOf<DirectionalLight>());
	light_data.point_light_count       = static_cast<u32>(ecs_engine.CountOf<PointLight>());
	light_data.spot_light_count        = static_cast<u32>(ecs_engine.CountOf<SpotLight>());

	light_data.fog_color = scene.GetFog().color;
	light_data.fog_start = scene.GetFog().start;
	light_data.fog_range = scene.GetFog().range;

	light_buffer.UpdateData(device_context, light_data);
}


void LightPass::UpdateDirectionalLightData(ECS& ecs_engine) {

	//auto& lights = scene.GetDirectionalLights();

	vector<DirectionalLightBuffer> buffers;
	buffers.reserve(ecs_engine.CountOf<DirectionalLight>());

	// Shadow light buffer

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
			// Shadow mapped light buffer
		}
	});

	directional_light_buffer.UpdateData(device, device_context, buffers);
}


void LightPass::UpdatePointLightData(ECS& ecs_engine) {

	vector<PointLightBuffer> buffers;
	buffers.reserve(ecs_engine.CountOf<PointLight>());

	ecs_engine.ForEachActive<PointLight>([&](PointLight& light) {

		const auto transform = ecs_engine.GetComponent<Transform>(light.GetOwner());

		if (!light.CastsShadows()) {
			PointLightBuffer buffer;

			XMStoreFloat3(&buffer.position, transform->GetPosition());
			buffer.ambient_color = light.GetAmbientColor();
			buffer.diffuse_color = light.GetDiffuseColor();
			buffer.specular      = light.GetSpecular();
			buffer.attenuation   = light.GetAttenuation();
			buffer.range         = light.GetRange();

			buffers.push_back(std::move(buffer));
		}
		else {
			// Shadow mapped light buffer
		}
	});

	point_light_buffer.UpdateData(device, device_context, buffers);
}


void LightPass::UpdateSpotLightData(ECS& ecs_engine) {

	vector<SpotLightBuffer> buffers;
	buffers.reserve(ecs_engine.CountOf<SpotLight>());

	ecs_engine.ForEachActive<SpotLight>([&](SpotLight& light) {

		const auto transform = ecs_engine.GetComponent<Transform>(light.GetOwner());

		if (!light.CastsShadows()) {
			SpotLightBuffer buffer;

			XMStoreFloat3(&buffer.position, transform->GetPosition());
			XMStoreFloat3(&buffer.direction, transform->GetWorldAxisZ());
			buffer.ambient_color = light.GetAmbientColor();
			buffer.diffuse_color = light.GetDiffuseColor();
			buffer.specular      = light.GetSpecular();
			buffer.attenuation   = light.GetAttenuation();
			buffer.spot          = light.GetSpot();
			buffer.range         = light.GetRange();

			buffers.push_back(std::move(buffer));
		}
		else {
			// Shadow mapped light buffer
		}
	});

	spot_light_buffer.UpdateData(device, device_context, buffers);
}