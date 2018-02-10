#pragma once

#include <vector>
#include <fstream>
#include <wrl\client.h>
#include <VertexTypes.h>
#include "util\EngineUtil.h"

using std::vector;
using std::ifstream;
using Microsoft::WRL::ComPtr;

using namespace DirectX;

class Model {
	public:
		Model();
		~Model();
		
		void Init(ID3D11Device* device, const char* modelFilename,
		             ComPtr<ID3D11ShaderResourceView> modelTexture, ShaderTypes shaderType);

		void RenderBuffers(ID3D11DeviceContext* deviceContext);

		void SetPosition(float x, float y, float z) {
			position = XMMatrixTranslation(x, y, z);
		}

		void Move(float x, float y, float z) {
			position = XMMatrixMultiply(position, XMMatrixTranslation(x, y, z));
		}

		void SetRotation(float x, float y, float z) {
			rotation = XMMatrixRotationRollPitchYaw(x, y, z);
		}

		void Rotate(float x, float y, float z) {
			rotation = XMMatrixMultiply(rotation, XMMatrixRotationRollPitchYaw(x, y, z));
		}

		int         GetIndexCount() { return indexCount; }
		XMMATRIX    GetPosition()   { return position; }
		XMMATRIX    GetRotation()   { return rotation; }
		ShaderTypes GetShader()     { return shader; }

		const ComPtr<ID3D11ShaderResourceView>& GetTexture() { return texture; }


	private:
		HRESULT LoadModel(const char* filename);
		HRESULT InitBuffers(ID3D11Device* device);


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
		XMMATRIX                         position;
		XMMATRIX                         rotation;
};

