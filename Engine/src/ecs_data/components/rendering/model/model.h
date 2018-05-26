#pragma once

#include "util\engine_util.h"
#include "datatypes\datatypes.h"
#include "io\io.h"
#include "math\math.h"


#include "component\component.h"
#include "rendering\buffer\buffers.h"
#include "rendering\buffer\constant_buffer.h"
#include "resource\mesh\mesh.h"
#include "resource\model\model_blueprint.h"
#include "resource\model\material\material.h"
#include "geometry\bounding_volume\bounding_volume.h"



class ModelChild final {
	friend class Model;

	protected:
		ModelChild(ID3D11Device& device, ModelPart& part, Material mat)
			: name(part.name)
			, buffer(device)
			, index_start(part.index_start)
			, index_count(part.index_count)
			, material(mat)
			, aabb(part.aabb)
			, sphere(part.sphere) {
		}

		void XM_CALLCONV UpdateBuffer(ID3D11DeviceContext& device_context,
									  FXMMATRIX world,
									  CXMMATRIX world_inv_transpose);

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
		void BindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
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




class Model final : public Component<Model> {
	public:
		Model(ID3D11Device& device, shared_ptr<ModelBlueprint> blueprint);
		~Model() = default;


		// Bind the model's vertex and index buffers
		void Bind(ID3D11DeviceContext& device_context) const {
			mesh->Bind(device_context);
		}

		// Render the model with the given index count and start index
		void Draw(ID3D11DeviceContext& device_context, u32 index_count, u32 start_index) const {
			mesh->Draw(device_context, index_count, start_index);
		}


		// Preform an action for each child model
		template<typename ActionT>
		void ForEachChild(ActionT act) {
			for (auto& child : child_models) {
				act(child);
			}
		}


		// Update model matrix and bounding volumes, as well as those of the child models.
		void XM_CALLCONV UpdateBuffer(ID3D11DeviceContext& device_context, FXMMATRIX world);

		void XM_CALLCONV UpdateBoundingVolumes(FXMMATRIX world);


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		const string&         GetName()      const { return name; }
		const AABB&           GetAABB()      const { return aabb; }
		const BoundingSphere& GetSphere()    const { return sphere; }


	private:
		// The model's name
		string name;

		// The vertex and index buffer of the model
		shared_ptr<Mesh> mesh;

		// Bounding volumes
		AABB aabb;
		BoundingSphere sphere;
		
		// The child models that make up this model
		vector<ModelChild> child_models;
};