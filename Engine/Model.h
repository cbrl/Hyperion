#pragma once

#include <wrl\client.h>
#include <vector>
#include <fstream>
#include "EngineUtil.h"

using std::vector;
using std::ifstream;
using Microsoft::WRL::ComPtr;

using namespace DirectX;

class Model {
	public:
		Model();
		~Model();
		
		bool        Init(const ComPtr<ID3D11Device>& device, const char* modelFilename,
						 const ComPtr<ID3D11ShaderResourceView>& texture, ShaderTypes shader);

		void        RenderBuffers(const ComPtr<ID3D11DeviceContext>& deviceContext);
		int         GetIndexCount();
		const       ComPtr<ID3D11ShaderResourceView>& GetTexture();
		ShaderTypes GetShader();


	private:
		bool LoadModel(const char* filename);
		bool InitBuffers(const ComPtr<ID3D11Device>& device);


	private:
		struct Vertex {
			XMFLOAT3 position;
			XMFLOAT2 texture;
			XMFLOAT3 normal;
		};

		struct ModelData {
			float x, y, z;
			float tu, tv;
			float nx, ny, nz;
		};


	private:
		int               m_VertexCount;
		int               m_IndexCount;
		vector<ModelData> m_ModelData;

		ComPtr<ID3D11Buffer>             m_VertexBuffer;
		ComPtr<ID3D11Buffer>             m_IndexBuffer;
		ComPtr<ID3D11ShaderResourceView> m_Texture;
		ShaderTypes                      m_Shader;
};

