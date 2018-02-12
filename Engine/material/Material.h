#pragma once

#include <string>
#include <wrl\client.h>
#include <d3d11.h>
#include <DirectXMath.h>

using std::wstring;
using Microsoft::WRL::ComPtr;

using namespace DirectX;

struct Material {
	Material() {
		name = L"material";
		Ns = 0.0f;
		Ni = 0.0f;
		d = 0.0f;
		illum = 0;
		transparent = false;
	}

	wstring name;

	// Ambient map
	ComPtr<ID3D11ShaderResourceView> map_Ka;
	// Diffuse map
	ComPtr<ID3D11ShaderResourceView> map_Kd;
	// Specular map
	ComPtr<ID3D11ShaderResourceView> map_Ks;
	// Specular Highlight map
	ComPtr<ID3D11ShaderResourceView> map_Ns;
	// Alpha map
	ComPtr<ID3D11ShaderResourceView> map_d;
	// Bump map
	ComPtr<ID3D11ShaderResourceView> map_bump;

	// Ambient Color
	XMFLOAT3 Ka;
	// Diffuse Color
	XMFLOAT3 Kd;
	// Specular Color
	XMFLOAT3 Ks;
	// Emissive Color
	XMFLOAT3 Ke;

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