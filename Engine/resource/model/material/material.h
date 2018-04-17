#pragma once

#include "util\math\math.h"
#include "util\datatypes\datatypes.h"
#include "resource\texture\texture.h"

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
		, reflection_enabled(false)
	{}


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
	float4 ambient;
	// Diffuse Color
	float4 diffuse;
	// Specular Color, w = spec exponent
	float4 specular;
	// Reflective color
	float4 reflect;
	// Emissive Color
	float4 emissive;

	// Optical Density (index of refraction)
	float optical_density;
	// Dissolve (alpha)
	float dissolve;
	// Illumination model
	i32 illum;

	// Transparency flag
	bool transparent;
	// Diffuse texture flag
	bool has_texture;
	// Relfection flag
	bool reflection_enabled;
};