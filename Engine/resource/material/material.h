#pragma once

#include "util\math\math.h"
#include "util\datatypes\datatypes.h"
#include "resource\texture\texture.h"

using namespace DirectX;

struct Material final {
	Material()
		: name(L"material")
		, Ka(0.0f, 0.0f, 0.0f, 1.0f)
		, Kd(0.0f, 0.0f, 0.0f, 1.0f)
		, Ks(0.0f, 0.0f, 0.0f, 1.0f)
		, Ke(0.0f, 0.0f, 0.0f, 1.0f)
		, Ni(0.0f)
		, d(0.0f)
		, illum(0)
		, transparent(false)
		, has_texture(true)
	{}


	// Material name
	wstring name;

	// Ambient map
	shared_ptr<Texture> map_Ka;
	// Diffuse map
	shared_ptr<Texture> map_Kd;
	// Specular map
	shared_ptr<Texture> map_Ks;
	// Specular Highlight map
	shared_ptr<Texture> map_Ns;
	// Alpha map
	shared_ptr<Texture> map_d;
	// Bump map
	shared_ptr<Texture> map_bump;

	// Ambient Color
	float4 Ka;
	// Diffuse Color
	float4 Kd;
	// Specular Color, w = spec exponent
	float4 Ks;
	// Emissive Color
	float4 Ke;

	// Optical Density
	float Ni;
	// Dissolve
	float d;
	// Illumination
	i32 illum;

	// Transparency flag
	bool transparent;

	// Diffuse texture flag
	bool has_texture;
};