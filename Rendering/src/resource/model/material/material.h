#pragma once

#include "directx/directx_math.h"
#include "datatypes/datatypes.h"
#include "resource/texture/texture.h"

using namespace DirectX;


struct Material final {
	Material()
		: name("material")
		, ambient(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, reflect(0.0f, 0.0f, 0.0f, 1.0f)
		, emissive(0.0f, 0.0f, 0.0f, 1.0f)
		, optical_density(0.0f)
		, dissolve(0.0f)
		, illum(0)
		, transparent(false)
		, has_texture(true)
		, reflection_enabled(false) {
	}


	// Material name
	string name;

	// Ambient map
	shared_ptr<Texture> map_ambient;
	// Diffuse map
	shared_ptr<Texture> map_diffuse;
	// Specular map
	shared_ptr<Texture> map_specular;
	// Specular Highlight map
	shared_ptr<Texture> map_spec_highlight;
	// Alpha map
	shared_ptr<Texture> map_alpha;
	// Bump map
	shared_ptr<Texture> map_bump;

	// Ambient Color
	f32_4 ambient;
	// Diffuse Color
	f32_4 diffuse;
	// Specular Color, w = spec exponent
	f32_4 specular;
	// Reflective color
	f32_4 reflect;
	// Emissive Color
	f32_4 emissive;

	// Optical Density (index of refraction)
	f32 optical_density;
	// Dissolve (alpha)
	f32 dissolve;
	// Illumination model
	i32 illum;

	// Transparency flag
	bool transparent;
	// Diffuse texture flag
	bool has_texture;
	// Relfection flag
	bool reflection_enabled;
};
