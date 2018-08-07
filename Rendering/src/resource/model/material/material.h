#pragma once

#include "directx/directx_math.h"
#include "datatypes/datatypes.h"
#include "resource/texture/texture.h"

using namespace DirectX;


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
