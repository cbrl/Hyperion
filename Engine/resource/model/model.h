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
		Model() = delete;
		~Model() = default;

		Model(ID3D11Device* device, ModelBlueprint blueprint)
			: name(blueprint.name)
			, mesh(blueprint.mesh)
			, model_parts(blueprint.model_parts)
			, materials(blueprint.materials)
			, aabb(blueprint.aabb)
			, sphere(blueprint.sphere)
			, position(XMMatrixTranslation(0.0f, 0.0f, 0.0f))
			, rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f))
			, scale(XMMatrixScaling(1.0f, 1.0f, 1.0f))
			, transform(XMMatrixIdentity())
		{}


		void Bind(ID3D11DeviceContext* device_context) const {
			mesh->Bind(device_context);
		}

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
			//UpdateBoundingVolumes();
		}

		void Move(float x, float y, float z) {
			position = XMMatrixMultiply(position, XMMatrixTranslation(x, y, z));
			//UpdateBoundingVolumes();
		}

		void SetRotation(float x, float y, float z) {
			rotation = XMMatrixRotationRollPitchYaw(x, y, z);
			//UpdateBoundingVolumes();
		}

		void Rotate(float x, float y, float z) {
			rotation = XMMatrixMultiply(rotation, XMMatrixRotationRollPitchYaw(x, y, z));
			//UpdateBoundingVolumes();
		}

		void SetScale(float x, float y, float z) {
			scale = XMMatrixScaling(x, y, z);
			//UpdateBoundingVolumes();
		}

		void Scale(float x, float y, float z) {
			scale = XMMatrixMultiply(scale, XMMatrixScaling(x, y, z));
			//UpdateBoundingVolumes();
		}

		void UpdateBoundingVolumes() {
			aabb.Transform(transform);
			sphere.Transform(transform);

			for (auto& part : model_parts) {
				part.aabb.Transform(transform);
				part.sphere.Transform(transform);
			}
		}


		//----------------------------------------------------------------------------------
		// Update model
		//----------------------------------------------------------------------------------

		void Update() {
			transform = XMMatrixMultiply(transform, (scale*rotation*position));

			UpdateBoundingVolumes();

			scale = rotation = position = XMMatrixIdentity();
		}


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		const AABB&           GetAABB()      const { return aabb; }
		const BoundingSphere& GetSphere()    const { return sphere; }
		const XMMATRIX&       GetTransform() const { return transform; }

		const Material& GetMaterial(u32 index) const { return materials[index]; }


	private:
		wstring name;

		AABB           aabb;
		BoundingSphere sphere;

		shared_ptr<Mesh> mesh;

		XMMATRIX position;
		XMMATRIX rotation;
		XMMATRIX scale;
		XMMATRIX transform;

		vector<ModelPart> model_parts;
		vector<Material>  materials;
};