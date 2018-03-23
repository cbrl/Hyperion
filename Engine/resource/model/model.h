#pragma once

#include "util\engine_util.h"
#include "util\io\io.h"
#include "util\math\math.h"
#include "util\datatypes\datatypes.h"
#include "resource\model\model_blueprint.h"
#include "resource\mesh\mesh.h"
#include "resource\material\material.h"
#include "geometry\boundingvolume\bounding_volume.h"


class Model {
	public:
		Model() = default;
		~Model() = default;

		Model(ID3D11Device* device, ModelBlueprint blueprint)
			: name(blueprint.name)
			, mesh(blueprint.mesh)
			, model_parts(blueprint.model_parts)
			, materials(blueprint.materials)
			, aabb(blueprint.aabb)
			, position(XMMatrixTranslation(0.0f, 0.0f, 0.0f))
			, rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f))
			, scale(XMMatrixScaling(1.0f, 1.0f, 1.0f))
		{}


		// Render the model with the given index count and start index
		void Draw(ID3D11DeviceContext* device_context, u32 index_count, u32 start_index) const {
			mesh->Draw(device_context, index_count, start_index);
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
		wstring name;
		AABB    aabb;

		shared_ptr<Mesh> mesh;

		XMMATRIX position;
		XMMATRIX rotation;
		XMMATRIX scale;

		vector<ModelPart> model_parts;
		vector<Material>  materials;
};