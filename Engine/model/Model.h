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
		
		bool Init(const ComPtr<ID3D11Device>& device, const char* modelFilename,
		          ComPtr<ID3D11ShaderResourceView> texture, ShaderTypes shader);

		void RenderBuffers(const ComPtr<ID3D11DeviceContext>& deviceContext);
		int  GetIndexCount();
		ShaderTypes GetShader();
		const ComPtr<ID3D11ShaderResourceView>& GetTexture();


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
		int               m_VertexCount;
		int               m_IndexCount;
		vector<ModelData> m_ModelData;

		ComPtr<ID3D11Buffer>             m_VertexBuffer;
		ComPtr<ID3D11Buffer>             m_IndexBuffer;
		ComPtr<ID3D11ShaderResourceView> m_Texture;
		Material                         m_Material;
		ShaderTypes                      m_Shader;
};

