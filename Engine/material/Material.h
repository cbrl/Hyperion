#pragma once

#include <string>
#include <wrl\client.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "texture\texture.h"

using std::wstring;
using std::shared_ptr;
using Microsoft::WRL::ComPtr;

using namespace DirectX;

struct Material {
	Material() :
		name(L"material"),
		Ka(0.0f, 0.0f, 0.0f, 1.0f),
		Kd(0.0f, 0.0f, 0.0f, 1.0f),
		Ks(0.0f, 0.0f, 0.0f, 1.0f),
		Ke(0.0f, 0.0f, 0.0f, 1.0f),
		Ns(1.0f),
		Ni(0.0f),
		d(0.0f),
		illum(0),
		transparent(false) {}


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
	XMFLOAT4 Ka;
	// Diffuse Color
	XMFLOAT4 Kd;
	// Specular Color
	XMFLOAT4 Ks;
	// Emissive Color
	XMFLOAT4 Ke;

	// Specular Exponent
	float Ns;
	// Optical Density
	float Ni;
	// Dissolve
	float d;
	// Illumination
	int illum;

	// Transparency flag
	bool transparent;
};