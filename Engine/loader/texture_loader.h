#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "util\engine_util.h"


class TextureLoader final {
	public:
		TextureLoader() = delete;

		// Load a texture from a file (jpg, png, etc...)
		static void LoadTexture(ID3D11Device* device,
								ID3D11DeviceContext* device_context,
								const wstring& filename,
								ID3D11ShaderResourceView** srv_out);

		// Create a texture from a specified color
		static void LoadTexture(ID3D11Device* device,
								u32 color,
								ID3D11ShaderResourceView** srv_out);

		// Load a Texture2DArray from multiple files
		static void LoadTexture(ID3D11Device* device,
								ID3D11DeviceContext* device_context,
								const vector<wstring>& filenames,
								ID3D11ShaderResourceView** srv_out);
};

