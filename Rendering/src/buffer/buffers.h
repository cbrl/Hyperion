#pragma once

#include "datatypes/datatypes.h"
#include "directx/directx_math.h"


//----------------------------------------------------------------------------------
// Model Buffer
//----------------------------------------------------------------------------------

struct MaterialBuffer {
	MaterialBuffer() noexcept
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

	MaterialBuffer(const vec4_f32& ambient,
	               const vec4_f32& diffuse,
	               const vec4_f32& specular,
	               const vec4_f32& reflect,
	               const vec4_f32& emissive,
	               const f32 optical_density,
	               const f32 dissolve,
	               const bool has_texture,
	               const bool reflection_enabled) noexcept
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

	vec4_f32 ambient;
	vec4_f32 diffuse;
	vec4_f32 specular;
	vec4_f32 reflect;
	vec4_f32 emissive;
	f32 optical_density;
	f32 dissolve;
	u32 has_texture;
	u32 reflection_enabled;
};


struct ModelBuffer {
	ModelBuffer() noexcept
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
	Fog() noexcept
		: color(0.0f, 0.0f, 0.0f, 1.0f)
		, start(0.0f)
		, range(0.0f) {
	}

	Fog(const vec4_f32& color,
	    const f32 start,
	    const f32 range) noexcept
		: color(color)
		, start(start)
		, range(range) {
	}

	vec4_f32 color;
	f32 start;
	f32 range;
};


struct LightBuffer {
	LightBuffer() noexcept
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
	            const vec4_f32& fog_color,
	            const f32 fog_start,
	            const f32 fog_range) noexcept
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

	vec4_f32 fog_color;
	f32 fog_start;
	f32 fog_range;
	vec2_f32 pad2;
};


struct DirectionalLightBuffer {
	DirectionalLightBuffer() noexcept
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, direction(0.0f, 0.0f, 0.0f)
		, pad(0.0f)
		, world_to_projection(XMMatrixIdentity()) {
	}

	DirectionalLightBuffer(const vec4_f32& ambient,
	                       const vec4_f32& diffuse,
	                       const vec3_f32& direction,
	                       const vec4_f32& specular,
	                       CXMMATRIX world_to_proj) noexcept
		: ambient_color(ambient)
		, diffuse_color(diffuse)
		, specular(specular)
		, direction(direction)
		, pad(0.0f)
		, world_to_projection(world_to_proj) {
	}

	vec4_f32 ambient_color;
	vec4_f32 diffuse_color;
	vec4_f32 specular; //specular.w is the specular power
	vec3_f32 direction;
	f32  pad;
	XMMATRIX world_to_projection;
};


struct PointLightBuffer {
	PointLightBuffer() noexcept
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, position(0.0f, 0.0f, 1.0f)
		, range(0.0f)
		, attenuation(0.0f, 0.0f, 0.0f)
		, pad(0.0f) {
	}

	PointLightBuffer(const vec4_f32& ambient_color,
	                 const vec4_f32& diffuse_color,
	                 const vec4_f32& specular,
	                 const vec3_f32& position,
	                 const f32 range,
	                 const vec3_f32& attenuation) noexcept
		: ambient_color(ambient_color)
		, diffuse_color(diffuse_color)
		, specular(specular)
		, position(position)
		, range(range)
		, attenuation(attenuation)
		, pad(0.0f) {
	}

	vec4_f32 ambient_color;
	vec4_f32 diffuse_color;
	vec4_f32 specular; //specular.w is the specular power
	vec3_f32 position;
	f32      range;
	vec3_f32 attenuation;
	f32 pad;
};


struct ShadowedPointLightBuffer {
	ShadowedPointLightBuffer() noexcept
		: world_to_light(XMMatrixIdentity())
		, projection_values(0.0f, 0.0f)
		, pad(0.0f, 0.0f) {
	}

	PointLightBuffer light_buffer;
	XMMATRIX world_to_light;
	vec2_f32 projection_values;
	vec2_f32 pad;
};


struct SpotLightBuffer {
	SpotLightBuffer() noexcept
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

	SpotLightBuffer(const vec4_f32& ambient_color,
	                const vec4_f32& diffuse_color,
	                const vec4_f32& specular,
	                const vec3_f32& position,
	                const f32 range,
	                const vec3_f32& direction,
	                const f32 cos_umbra,
	                const f32 cos_penumbra,
	                const vec3_f32& attenuation) noexcept
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

	vec4_f32 ambient_color;
	vec4_f32 diffuse_color;
	vec4_f32 specular; //specular.w is the specular power
	vec3_f32 position;
	f32      range;
	vec3_f32 direction;
	f32      cos_umbra;
	f32      cos_penumbra;
	vec3_f32 attenuation;
};


struct ShadowedSpotLightBuffer {
	ShadowedSpotLightBuffer() noexcept
		: world_to_projection(XMMatrixIdentity()) {
	}

	SpotLightBuffer light_buffer;
	XMMATRIX        world_to_projection;
};



//----------------------------------------------------------------------------------
// Camera Buffer
//----------------------------------------------------------------------------------

struct CameraBuffer {
	CameraBuffer() noexcept
		: camera_to_world(XMMatrixIdentity())
		, world_to_camera(XMMatrixIdentity())
		, camera_to_projection(XMMatrixIdentity()) {
	}

	CameraBuffer(CXMMATRIX camera_to_world,
	             CXMMATRIX world_to_camera,
	             CXMMATRIX camera_to_projection,
	             const Fog& fog) noexcept
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
	AltCameraBuffer() noexcept
		: world_to_camera(XMMatrixIdentity())
		, camera_to_projection(XMMatrixIdentity()) {
	}

	AltCameraBuffer(CXMMATRIX world_to_cam, CXMMATRIX cam_to_proj) noexcept
		: world_to_camera(world_to_cam)
		, camera_to_projection(cam_to_proj) {
	}

	XMMATRIX world_to_camera;
	XMMATRIX camera_to_projection;
};