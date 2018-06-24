#pragma once

#include "datatypes/datatypes.h"
#include "directx/directx_math.h"


//----------------------------------------------------------------------------------
// Model Buffer
//----------------------------------------------------------------------------------

struct MaterialBuffer {
	MaterialBuffer()
		: ambient(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, reflect(0.0f, 0.0f, 0.0f, 1.0f)
		, emissive(0.0f, 0.0f, 0.0f, 1.0f)
		, optical_density(0.0f)
		, dissolve(0.0f)
		, has_texture(0)
		, reflection_enabled(0) {
	}

	MaterialBuffer(const f32_4& ambient,
	               const f32_4& diffuse,
	               const f32_4& specular,
	               const f32_4& reflect,
	               const f32_4& emissive,
	               const f32 optical_density,
	               const f32 dissolve,
	               const bool has_texture,
	               const bool reflection_enabled)
		: ambient(ambient)
		, diffuse(diffuse)
		, specular(specular)
		, reflect(reflect)
		, emissive(emissive)
		, optical_density(optical_density)
		, dissolve(dissolve)
		, has_texture(has_texture)
		, reflection_enabled(reflection_enabled) {
	}

	f32_4 ambient;
	f32_4 diffuse;
	f32_4 specular;
	f32_4 reflect;
	f32_4 emissive;
	f32  optical_density;
	f32  dissolve;
	u32    has_texture;
	u32    reflection_enabled;
};


struct ModelBuffer {
	ModelBuffer()
		: world(XMMatrixIdentity())
		, world_inv_transpose(XMMatrixIdentity())
		, texTransform(XMMatrixIdentity()) {
	}

	XMMATRIX       world;
	XMMATRIX       world_inv_transpose;
	XMMATRIX       texTransform;
	MaterialBuffer mat;
};


//----------------------------------------------------------------------------------
// Light Buffers
//----------------------------------------------------------------------------------

struct Fog {
	Fog()
		: color(0.0f, 0.0f, 0.0f, 1.0f)
		, start(0.0f)
		, range(0.0f) {
	}

	Fog(const f32_4& color,
	    const f32 start,
	    const f32 range)
		: color(color)
		, start(start)
		, range(range) {
	}

	f32_4 color;
	f32 start;
	f32 range;
};


struct LightBuffer {
	LightBuffer()
		: num_directional_lights(0)
		, num_point_lights(0)
		, num_spot_lights(0)
		, pad0(0)
		, num_shadow_directional_lights(0)
		, num_shadow_point_lights(0)
		, num_shadow_spot_lights(0)
		, pad1(0)
		, fog_color(1.0f, 1.0f, 1.0f, 1.0f)
		, fog_start(0.0f)
		, fog_range(0.0f)
		, pad2(0.0f, 0.0f) {
	}

	LightBuffer(const u32 directional_lights,
	            const u32 point_lights,
	            const u32 spot_lights,
	            const u32 shadowed_directional_lights,
	            const u32 shadowed_point_lights,
	            const u32 shadowed_spot_lights,
	            const f32_4& fog_color,
	            const f32 fog_start,
	            const f32 fog_range)
		: num_directional_lights(directional_lights)
		, num_point_lights(point_lights)
		, num_spot_lights(spot_lights)
		, pad0(0)
		, num_shadow_directional_lights(shadowed_directional_lights)
		, num_shadow_point_lights(shadowed_point_lights)
		, num_shadow_spot_lights(shadowed_spot_lights)
		, pad1(0)
		, fog_color(fog_color)
		, fog_start(fog_start)
		, fog_range(fog_range)
		, pad2(0.0f, 0.0f) {
	}

	u32 num_directional_lights;
	u32 num_point_lights;
	u32 num_spot_lights;
	u32 pad0;

	u32 num_shadow_directional_lights;
	u32 num_shadow_point_lights;
	u32 num_shadow_spot_lights;
	u32 pad1;

	f32_4 fog_color;
	f32  fog_start;
	f32  fog_range;
	f32_2 pad2;
};


struct DirectionalLightBuffer {
	DirectionalLightBuffer()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, direction(0.0f, 0.0f, 0.0f)
		, pad(0.0f) {
	}

	DirectionalLightBuffer(const f32_4& ambient,
	                       const f32_4& diffuse,
	                       const f32_3& direction,
	                       const f32_4& specular,
	                       CXMMATRIX world_to_proj)
		: ambient_color(ambient)
		, diffuse_color(diffuse)
		, specular(specular)
		, direction(direction)
		, pad(0.0f)
		, world_to_projection(world_to_proj) {
	}

	f32_4 ambient_color;
	f32_4 diffuse_color;
	f32_4 specular; //specular.w is the specular power
	f32_3 direction;
	f32  pad;
	XMMATRIX world_to_projection;
};


struct PointLightBuffer {
	PointLightBuffer()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, position(0.0f, 0.0f, 1.0f)
		, range(0.0f)
		, attenuation(0.0f, 0.0f, 0.0f)
		, pad(0.0f) {
	}

	PointLightBuffer(const f32_4& ambient_color,
	                 const f32_4& diffuse_color,
	                 const f32_4& specular,
	                 const f32_3& position,
	                 const f32 range,
	                 const f32_3& attenuation)
		: ambient_color(ambient_color)
		, diffuse_color(diffuse_color)
		, specular(specular)
		, position(position)
		, range(range)
		, attenuation(attenuation)
		, pad(0.0f) {
	}

	f32_4 ambient_color;
	f32_4 diffuse_color;
	f32_4 specular; //specular.w is the specular power
	f32_3 position;
	f32  range;
	f32_3 attenuation;
	f32  pad;
};


struct ShadowedPointLightBuffer {
	ShadowedPointLightBuffer()
		: world_to_light(XMMatrixIdentity())
		, projection_values(0.0f, 0.0f)
		, pad(0.0f, 0.0f) {
	}

	PointLightBuffer light_buffer;
	XMMATRIX world_to_light;
	f32_2 projection_values;
	f32_2 pad;
};


struct SpotLightBuffer {
	SpotLightBuffer()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, position(0.0f, 0.0f, 0.0f)
		, range(0.0f)
		, direction(0.0f, 0.0f, 0.0f)
		, cos_umbra(0.0f)
		, cos_penumbra(0.0f)
		, attenuation(0.0f, 0.0f, 0.0f) {
	}

	SpotLightBuffer(const f32_4& ambient_color,
	                const f32_4& diffuse_color,
	                const f32_4& specular,
	                const f32_3& position,
	                const f32 range,
	                const f32_3& direction,
	                const f32 cos_umbra,
	                const f32 cos_penumbra,
	                const f32_3& attenuation)
		: ambient_color(ambient_color)
		, diffuse_color(diffuse_color)
		, specular(specular)
		, position(position)
		, range(range)
		, direction(direction)
		, cos_umbra(cos_umbra)
		, cos_penumbra(cos_penumbra)
		, attenuation(attenuation) {
	}

	f32_4 ambient_color;
	f32_4 diffuse_color;
	f32_4 specular; //specular.w is the specular power
	f32_3 position;
	f32  range;
	f32_3 direction;
	f32  cos_umbra;
	f32  cos_penumbra;
	f32_3 attenuation;
};


struct ShadowedSpotLightBuffer {
	ShadowedSpotLightBuffer()
		: world_to_projection(XMMatrixIdentity()) {
	}

	SpotLightBuffer light_buffer;
	XMMATRIX        world_to_projection;
};



//----------------------------------------------------------------------------------
// Camera Buffer
//----------------------------------------------------------------------------------

struct CameraBuffer {
	CameraBuffer()
		: camera_to_world(XMMatrixIdentity())
		, world_to_camera(XMMatrixIdentity())
		, camera_to_projection(XMMatrixIdentity()) {
	}

	CameraBuffer(CXMMATRIX camera_to_world,
	             CXMMATRIX world_to_camera,
	             CXMMATRIX camera_to_projection,
	             const Fog& fog)
		: camera_to_world(camera_to_world)
		, world_to_camera(world_to_camera)
		, camera_to_projection(camera_to_projection)
		, fog(fog) {
	}

	XMMATRIX camera_to_world;
	XMMATRIX world_to_camera;
	XMMATRIX camera_to_projection;
	Fog      fog;
};


struct AltCameraBuffer {
	AltCameraBuffer()
		: world_to_camera(XMMatrixIdentity())
		, camera_to_projection(XMMatrixIdentity()) {
	}

	AltCameraBuffer(CXMMATRIX world_to_cam, CXMMATRIX cam_to_proj)
		: world_to_camera(world_to_cam)
		, camera_to_projection(cam_to_proj) {
	}

	XMMATRIX world_to_camera;
	XMMATRIX camera_to_projection;
};