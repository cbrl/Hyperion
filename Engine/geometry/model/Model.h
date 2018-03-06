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
		      const vector<u32>& material_indices, u32 group_count, const vector<u32>& group_indices);


		// Construct the model
		template<typename VertexT>
		void Init(ID3D11Device* device, const vector<VertexT>& vertices, const vector<u32>& indices, const vector<Material>& materials,
		          const vector<u32>& material_indices, u32 group_count, const vector<u32>& group_indices);


		// Render the model with the given index count and start index
		void Draw(ID3D11DeviceContext* device_context, u32 index_count, u32 start_index) const {
			mesh.Draw(device_context, index_count, start_index);
		}


		// Preform an action for each part of the model
		template<typename ActionT>
		void ForEachPart(ActionT act) const {
			for (const auto& part : model_parts) {
				act(part);
			}
		}


		//----------------------------------------------------------------------------------
		// Move / Rotate / Scale
		//----------------------------------------------------------------------------------

		void SetPosition(float x, float y, float z) {
			position = XMMatrixTranslation(x, y, z);
			UpdateAABBs();
		}

		void Move(float x, float y, float z) {
			position = XMMatrixMultiply(position, XMMatrixTranslation(x, y, z));
			UpdateAABBs();
		}

		void SetRotation(float x, float y, float z) {
			rotation = XMMatrixRotationRollPitchYaw(x, y, z);
			UpdateAABBs();
		}

		void Rotate(float x, float y, float z) {
			rotation = XMMatrixMultiply(rotation, XMMatrixRotationRollPitchYaw(x, y, z));
			UpdateAABBs();
		}

		void SetScale(float x, float y, float z) {
			scale = XMMatrixScaling(x, y, z);
			UpdateAABBs();
		}

		void Scale(float x, float y, float z) {
			scale = XMMatrixMultiply(scale, XMMatrixScaling(x, y, z));
			UpdateAABBs();
		}

		void UpdateAABBs() {
			aabb.Transform(scale * rotation * position);
			for (auto& part : model_parts) {
				part.aabb.Transform(scale * rotation * position);
			}
		}


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

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


#include "model.tpp"