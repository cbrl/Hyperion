#pragma once

#include <vector>
#include <fstream>
#include <memory>
#include <wrl\client.h>
#include <VertexTypes.h>
#include "util\EngineUtil.h"
#include "geometry\mesh\Mesh.h"
#include "geometry\boundingvolume\BoundingVolume.h"

using std::vector;
using std::ifstream;
using std::unique_ptr;
using Microsoft::WRL::ComPtr;

using namespace DirectX;

class Model {
	public:
		Model(const Mesh& mesh, const AABB& aabb);
		~Model();

		void SetShader(ShaderTypes shaderType) { shader = shaderType; }

		void Draw(ID3D11DeviceContext* deviceContext);

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

		AABB        GetAABB()     const { return aabb; }
		XMMATRIX    GetPosition() const { return position; }
		XMMATRIX    GetRotation() const { return rotation; }
		ShaderTypes GetShader()   const { return shader; }


	private:
		Mesh mesh;
		AABB aabb;

		ShaderTypes  shader;
		XMMATRIX     position;
		XMMATRIX     rotation;
};