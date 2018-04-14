#pragma once

#include "util\engine_util.h"
#include "util\io\io.h"
#include "util\math\math.h"
#include "util\datatypes\datatypes.h"

#include "resource\model\model_blueprint.h"
#include "resource\mesh\mesh.h"
#include "resource\material\material.h"
#include "geometry\transform\transform.h"
#include "geometry\bounding_volume\bounding_volume.h"
#include "rendering\buffer\constant_buffer.h"



class ModelChild final {
	friend class Model;

	protected:
		ModelChild(ID3D11Device* device, ModelPart& part, Material mat)
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
								CXMMATRIX world_view_proj);

		void XM_CALLCONV UpdateBoundingVolumes(FXMMATRIX transform) {
			aabb.Transform(transform);
			sphere.Transform(transform);
		}


	public:
		~ModelChild() = default;

		const u32 GetIndexStart() const { return index_start; }
		const u32 GetIndexCount() const { return index_count; }

		const string&         GetName()     const { return name; }
		const AABB&           GetAABB()     const { return aabb; }
		const BoundingSphere& GetSphere()   const { return sphere; }
		const Material&       GetMaterial() const { return material; }

		template<typename StageT>
		void BindBuffer(ID3D11DeviceContext* device_context, u32 slot) const {
			buffer.Bind<StageT>(device_context, slot);
		}


	private:
		// The name of the model child
		string name;

		// Index info
		u32 index_start;
		u32 index_count;

		// Bounding volumes
		AABB aabb;
		BoundingSphere sphere;

		// Material
		Material material;

		// The cbuffer for this model child
		ConstantBuffer<ModelBuffer> buffer;
};




class Model final {
	public:
		Model(ID3D11Device* device, shared_ptr<ModelBlueprint> blueprint);
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
		void ForEachChild(ActionT act) {
			for (auto& child : child_models) {
				act(child);
			}
		}


		//----------------------------------------------------------------------------------
		// Move / Rotate / Scale
		//----------------------------------------------------------------------------------

		void SetPosition(const float3& position) {
			update_bounding_volumes = true;
			transform.SetPosition(position);
		}

		void Move(const float3& position) {
			update_bounding_volumes = true;
			transform.Move(position);
		}

		void SetRotation(const float3& rotation) {
			update_bounding_volumes = true;
			transform.SetRotation(rotation);
		}

		void Rotate(const float3& rotation) {
			update_bounding_volumes = true;
			transform.Rotate(rotation);
		}

		void SetScale(const float3& scale) {
			update_bounding_volumes = true;
			transform.SetScale(scale);
		}

		void Scale(const float3& scale) {
			update_bounding_volumes = true;
			transform.Scale(scale);
		}


		// Update model matrix and bounding volumes, as well as those of the child models.
		void XM_CALLCONV Update(ID3D11DeviceContext* device_context, FXMMATRIX view, CXMMATRIX proj);


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		const string&         GetName()      const { return name; }
		const AABB&           GetAABB()      const { return aabb; }
		const BoundingSphere& GetSphere()    const { return sphere; }
		const Transform&      GetTransform() const { return transform; }


	private:
		// The model's name
		string name;

		// The vertex and index buffer of the model
		shared_ptr<Mesh> mesh;

		// Bounding volumes
		AABB aabb;
		BoundingSphere sphere;

		// The model's transform
		Transform transform;
		
		// The child models that make up this model
		vector<ModelChild> child_models;

		// Flag that's set when the model transform changes
		bool update_bounding_volumes;
};