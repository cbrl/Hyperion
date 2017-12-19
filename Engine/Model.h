#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl\client.h>
#include <vector>
#include <fstream>

using Microsoft::WRL::ComPtr;
using std::vector;
using std::ifstream;
using namespace DirectX;

class Model {
	public:
		Model();
		~Model();
		
		bool Init(ComPtr<ID3D11Device> &device, char *modelFilename, ComPtr<ID3D11ShaderResourceView> texture);
		bool LoadModel(char *filename);
		bool InitBuffers(ComPtr<ID3D11Device> &device);
		void RenderBuffers(ComPtr<ID3D11DeviceContext> &deviceContext);


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
		char    *m_modelFilename;
		int      m_VertexCount;
		int      m_IndexCount;

		vector<ModelData> m_Model;

		ComPtr<ID3D11Buffer> m_VertexBuffer;
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		ComPtr<ID3D11ShaderResourceView> m_Texture;
};

