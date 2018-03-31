#pragma once

#include "util\engine_util.h"
#include "util\io\io.h"
#include "util\math\math.h"
#include "util\datatypes\datatypes.h"

#include "resource\model\model_blueprint.h"
#include "resource\mesh\mesh.h"
#include "resource\material\material.h"
#include "geometry\boundingvolume\bounding_volume.h"
#include "rendering\buffer\constant_buffer.h"



class ChildModel final {
	friend class Model;

	protected:
		ChildModel(ID3D11Device* device, ModelPart& part, shared_ptr<Material> mat)
			: name(part.name)
			, buffer(device)
			, index_start(part.index_start)
			, index_count(part.index_count)
			, material(mat)
			, aabb(part.aabb)
			, sphere(part.sphere) {
		}

		void XM_CALLCONV Update(ID3D11DeviceContext* device_context,
								FXMMATRIX world,
								CXMMATRIX world_inv_transpose,
								CXMMATRIX world_view_proj,
								CXMMATRIX tex_transform);

		void XM_CALLCONV UpdateBoundingVolumes(FXMMATRIX transform) {
			aabb.Transform(transform);
			sphere.Transform(transform);
		}


	public:
		~ChildModel() = default;

		const u32 GetIndexStart() const { return index_start; }
		const u32 GetIndexCount() const { return index_count; }

		const AABB&           GetAABB()     const { return aabb; }
		const BoundingSphere& GetSphere()   const { return sphere; }
		const Material&       GetMaterial() const { return *material; }

		template<typename StageT>
		void BindBuffer(ID3D11DeviceContext* device_context, u32 slot) const {
			buffer.Bind<StageT>(device_context, slot);
		}


	private:
		wstring name;

		ConstantBuffer<ModelBuffer> buffer;

		u32 index_start;
		u32 index_count;

		shared_ptr<Material> material;

		AABB           aabb;
		BoundingSphere sphere;
};


class Model final {
	public:
		Model(ID3D11Device* device, ModelBlueprint blueprint);
		~Model() = default;


		// Bind the model's vertex and index buffers
		void Bind(ID3D11DeviceContext* device_context) const {
			mesh->Bind(device_context);
		}

		// Render the model with the given index count and start index
		void Draw(ID3D11DeviceContext* device_context, u32 index_count, u32 start_index) const {
			mesh->Draw(device_context, index_count, start_index);
		}


		// Preform an action for each child model
		template<typename ActionT>
		void ForEachChildModel(ActionT act) {
			for (auto& child : child_models) {
				act(child);
			}
		}


		//----------------------------------------------------------------------------------
		// Move / Rotate / Scale
		//----------------------------------------------------------------------------------

		void SetPosition(float x, float y, float z) {
			position = XMMatrixTranslation(x, y, z);
			update_bounding_volumes = true;
		}

		void Move(float x, float y, float z) {
			position = XMMatrixMultiply(position, XMMatrixTranslation(x, y, z));
			update_bounding_volumes = true;
		}

		void SetRotation(float x, float y, float z) {
			rotation = XMMatrixRotationRollPitchYaw(x, y, z);
			update_bounding_volumes = true;
		}

		void Rotate(float x, float y, float z) {
			rotation = XMMatrixMultiply(rotation, XMMatrixRotationRollPitchYaw(x, y, z));
			update_bounding_volumes = true;
		}

		void SetScale(float x, float y, float z) {
			scale = XMMatrixScaling(x, y, z);
			update_bounding_volumes = true;
		}

		void Scale(float x, float y, float z) {
			scale = XMMatrixMultiply(scale, XMMatrixScaling(x, y, z));
			update_bounding_volumes = true;
		}


		// Update model matrix and bounding volumes, as well as those of the child models.
		void XM_CALLCONV Update(ID3D11DeviceContext* device_context, FXMMATRIX view, CXMMATRIX proj);


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		const AABB&           GetAABB()      const { return aabb; }
		const BoundingSphere& GetSphere()    const { return sphere; }
		const XMMATRIX&       GetTransform() const { return transform; }


	private:
		wstring name;

		AABB           aabb;
		BoundingSphere sphere;

		shared_ptr<Mesh> mesh;

		vector<ChildModel> child_models;

		vector<shared_ptr<Material>> materials;

		XMMATRIX position;
		XMMATRIX rotation;
		XMMATRIX scale;
		XMMATRIX transform;
		bool     update_bounding_volumes;
};