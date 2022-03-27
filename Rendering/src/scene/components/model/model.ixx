module;

#include "datatypes/types.h"
#include "io/io.h"

#include "directx/d3d11.h"
#include "directxmath/directxmath.h"

export module components.model;

import ecs;
import rendering.buffer_types;
import rendering.constant_buffer;
export import rendering.model_blueprint;


//----------------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------------
//
// A model is a collection of a mesh, material, bounding volumes, and a shader
// constant buffer. It is created from a ModelBlueprint.
//
//----------------------------------------------------------------------------------
export class Model final : public ecs::Component {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Model(ID3D11Device& device,
	      const std::shared_ptr<render::ModelBlueprint>& bp,
	      u32 bp_index)
		: Model(device,
		        bp->meshes.at(bp_index).getName(),
		        bp->meshes.at(bp_index),
		        bp->materials.at(bp->mat_indices.at(bp_index)),
		        bp->aabbs.at(bp_index),
		        bp->bounding_spheres.at(bp_index),
		        bp) {
	}

	Model(ID3D11Device& device,
	      const std::string& name,
	      const render::Mesh& mesh,
	      render::Material& mat,
	      const AABB& aabb,
	      const BoundingSphere& sphere,
	      const std::shared_ptr<render::ModelBlueprint>& bp)
		: buffer(device)
		, name(name)
		, mesh(mesh)
		, material(mat)
		, aabb(aabb)
		, bounding_sphere(sphere)
		, shadows(true)
		, blueprint(bp) {
	}


	Model(const Model&) = delete;
	Model(Model&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Model() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Model& operator=(const Model&) = delete;
	Model& operator=(Model&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Bind
	//----------------------------------------------------------------------------------

	void bindMesh(ID3D11DeviceContext& device_context) const {
		mesh.get().bind(device_context);
	}

	template<typename StageT>
	void bindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
		buffer.bind<StageT>(device_context, slot);
	}

	void XM_CALLCONV updateBuffer(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world){
		// The model-to-world matrix. Transposed for HLSL.
		const auto world_t = XMMatrixTranspose(object_to_world);

		// Create the inverse transpose of the model-to-world matrix
		const auto world_inv_t = XMMatrixInverse(nullptr, object_to_world);

		render::ModelBuffer buffer_data;

		buffer_data.world               = world_t;
		buffer_data.world_inv_transpose = world_inv_t;
		buffer_data.tex_transform       = XMMatrixIdentity();

		buffer_data.mat.base_color = material.get().params.base_color;
		buffer_data.mat.metalness  = material.get().params.metalness;
		buffer_data.mat.roughness  = material.get().params.roughness;
		buffer_data.mat.emissive   = material.get().params.emissive;

		buffer.updateData(device_context, buffer_data);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Name
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const std::string getName() const noexcept {
		return name;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Vertices
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	u32 getVertexCount() const noexcept {
		return mesh.get().getVertexCount();
	}

	[[nodiscard]]
	u32 getIndexCount() const noexcept {
		return mesh.get().getIndexCount();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Material
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	render::Material& getMaterial() noexcept {
		return material;
	}

	[[nodiscard]]
	const render::Material& getMaterial() const noexcept {
		return material;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Bounding Volumes
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const AABB& getAABB() const noexcept {
		return aabb;
	}

	[[nodiscard]]
	const BoundingSphere& getBoundingSphere() const noexcept {
		return bounding_sphere;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Shadows
	//----------------------------------------------------------------------------------

	void setShadows(bool state) noexcept {
		shadows = state;
	}

	[[nodiscard]]
	bool castsShadows() const noexcept {
		return shadows;
	}


	[[nodiscard]]
	const render::ModelBlueprint& getBlueprint() const noexcept {
		return *blueprint;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The name of the model
	std::string name;

	// The shader constant buffer
	render::ConstantBuffer<render::ModelBuffer> buffer;

	// The mesh that the model refers to
	std::reference_wrapper<const render::Mesh> mesh;

	// The material that the model refers to
	std::reference_wrapper<render::Material> material;

	// The bounding volumes of the model
	AABB aabb;
	BoundingSphere bounding_sphere;

	// A flag that determines if the model casts shadows
	bool shadows;

	// The blueprint whose data this model references
	std::shared_ptr<render::ModelBlueprint> blueprint;
};
