#pragma once

#include <VertexTypes.h>
#include <wrl\client.h>
#include <vector>
#include <fstream>
#include "util\EngineUtil.h"

using std::vector;
using std::ifstream;
using Microsoft::WRL::ComPtr;

using namespace DirectX;

class Model {
	public:
		Model();
		~Model();
		
		bool Init(ID3D11Device* device, const char* modelFilename,
		          ComPtr<ID3D11ShaderResourceView> modelTexture, ShaderTypes shaderType);

		void RenderBuffers(const ComPtr<ID3D11DeviceContext>& deviceContext);

		int         GetIndexCount() { return indexCount; }
		ShaderTypes GetShader()     { return shader; }

		const ComPtr<ID3D11ShaderResourceView>& GetTexture() { return texture; }


	private:
		bool LoadModel(const char* filename);
		bool InitBuffers(const ComPtr<ID3D11Device>& device);


	private:
		struct ModelData {
			float x, y, z;
			float nx, ny, nz;
			float tu, tv;
		};

		struct Material {
			XMFLOAT4 diffuse;
			XMFLOAT4 ambient;
			XMFLOAT4 specular;
		};


	private:
		int               vertexCount;
		int               indexCount;
		vector<ModelData> modelData;

		ComPtr<ID3D11Buffer>             vertexBuffer;
		ComPtr<ID3D11Buffer>             indexBuffer;
		ComPtr<ID3D11ShaderResourceView> texture;
		Material                         material;
		ShaderTypes                      shader;
};

