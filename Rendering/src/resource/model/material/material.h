#pragma once

#include "directx/directx_math.h"
#include "datatypes/datatypes.h"
#include "resource/texture/texture.h"

using namespace DirectX;

/*
struct Material final {
	Material() noexcept
		: name("material")
		, ambient(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, reflect(0.0f, 0.0f, 0.0f, 1.0f)
		, emissive(0.0f, 0.0f, 0.0f, 1.0f)
		, optical_density(0.0f)
		, illum(0)
		, transparent(false)
		, has_texture(true)
		, reflection_enabled(false) {
	}


	// Material name
	std::string name;

	// Ambient std::map
	shared_ptr<Texture> map_ambient;
	// Diffuse std::map
	shared_ptr<Texture> map_diffuse;
	// Specular std::map
	shared_ptr<Texture> map_specular;
	// Specular Highlight std::map
	shared_ptr<Texture> map_spec_highlight;
	// Alpha std::map
	shared_ptr<Texture> map_alpha;
	// Bump std::map
	shared_ptr<Texture> map_bump;

	// Ambient Color
	vec4_f32 ambient;
	// Diffuse Color
	vec4_f32 diffuse;
	// Specular Color, w = spec exponent
	vec4_f32 specular;
	// Reflective color
	vec4_f32 reflect;
	// Emissive Color
	vec4_f32 emissive;

	// Optical Density (index of refraction)
	f32 optical_density;
	// Illumination model
	i32 illum;

	// Transparency flag
	bool transparent;
	// Diffuse texture flag
	bool has_texture;
	// Relfection flag
	bool reflection_enabled;
};
*/


struct Material {

	std::string name;

	struct {
		vec4_f32 diffuse;
		vec4_f32 ambient;
		vec4_f32 specular;
		vec4_f32 emissive;
		vec4_f32 transparent;
		
		/*TODO*/ bool  wireframe        = false;
		/*TODO*/ bool  two_sided        = false;
		bool  reflection       = false;
		float opacity          = 1.0f;
		float spec_scale       = 1.0f;
		float spec_exponent    = 1.0f;
		float refraction_index = 1.0f;
	} params;

	struct {
		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> ambient;
		std::shared_ptr<Texture> normal;
		std::shared_ptr<Texture> specular;
		std::shared_ptr<Texture> spec_exponent;
		std::shared_ptr<Texture> emissive;
		std::shared_ptr<Texture> opacity;
		std::shared_ptr<Texture> height;
		std::shared_ptr<Texture> light;
	} maps;
};