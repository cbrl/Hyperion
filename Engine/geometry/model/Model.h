#pragma once

#include "util\engine_util.h"
#include "util\io\io.h"
#include "util\math\math.h"
#include "util\datatypes\datatypes.h"
#include "geometry\mesh\vertex_types.h"
#include "material\material.h"
#include "geometry\mesh\mesh.h"
#include "geometry\boundingvolume\bounding_volume.h"


using namespace DirectX;


struct ModelPart {
	u32  index_start;
	u32  index_count;
	u32  material_index;
	AABB aabb;
};


class Model {
	public:
		Model() = default;
		~Model() = default;

		Model(const Mesh& mesh, const AABB& aabb)
			: position(XMMatrixTranslation(0.0f, 0.0f, 0.0f))
			, rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f))
			, scale(XMMatrixScaling(1.0f, 1.0f, 1.0f))
			, mesh(mesh)
			, aabb(aabb)
		{}

		template<typename VertexT>
		Model(ID3D11Device* device, const vector<VertexT>& vertices, const vector<u32>& indices, const vector<Material>& materials,
			  u32 group_count, const vector<u32>& group_indices, const vector<u32>& material_indices);

		void Draw(ID3D11DeviceContext* device_context, u32 index_count, u32 start_index) const {
			mesh.Draw(device_context, index_count, start_index);
		}

		template<typename ActionT>
		void ForEachPart(ActionT act) const {
			for (const auto& part : model_parts) {
				act(part);
			}
		}

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

		void SetScale(float x, float y, float z) {
			scale = XMMatrixMultiply(scale, XMMatrixScaling(x, y, z));
		}

		const AABB&     GetAABB()     const { return aabb; }
		const XMMATRIX& GetPosition() const { return position; }
		const XMMATRIX& GetRotation() const { return rotation; }
		const XMMATRIX& GetScale()    const { return scale; }

		const Material& GetMaterial(u32 index) const { return materials[index]; }


	private:
		Mesh mesh;
		AABB aabb;

		XMMATRIX position;
		XMMATRIX rotation;
		XMMATRIX scale;

		vector<ModelPart> model_parts;
		vector<Material>  materials;
};


template<typename VertexT>
Model::Model(ID3D11Device* device, const vector<VertexT>& vertices, const vector<u32>& indices,
			 const vector<Material>& materials, u32 group_count, const vector<u32>& group_indices,
			 const vector<u32>& material_indices)
	: position(XMMatrixTranslation(0.0f, 0.0f, 0.0f))
	, rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f))
	, scale(XMMatrixScaling(1.0f, 1.0f, 1.0f))
	, materials(materials)
{
	// Create the mesh
	mesh = Mesh(device, vertices, indices);

	// Create the AABB for the model
	auto pair = MinMaxPoint(vertices);
	aabb = AABB(pair.first, pair.second);

	for (size_t i = 0; i < group_count; ++i) {
		ModelPart temp;

		temp.index_start = group_indices[i];
		temp.material_index = material_indices[i];

		// Index count
		if (i == group_count - 1)
			temp.index_count = static_cast<u32>(indices.size() - group_indices[i]);
		else
			temp.index_count = group_indices[i + 1] - group_indices[i];

		// Create the AABB for the model part
		vector<VertexT> subvec;
		for (size_t j = temp.index_start; j < (temp.index_start + temp.index_count); ++j) {
			subvec.push_back(vertices[indices[j]]);
		}
		auto pair = MinMaxPoint(subvec);
		/*auto begin = vertices.begin() + indices[temp.index_start];
		auto end = vertices.begin() + indices[temp.index_start + temp.index_count - 1];
		auto pair = MinMaxPoint(vector<VertexT>(begin, end));*/
		temp.aabb = AABB(pair.first, pair.second);

		model_parts.push_back(temp);
	}
}